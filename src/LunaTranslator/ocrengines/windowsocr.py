import gobject
import winrtutils, windows, re, os
from myutils.hwnd import subprochiderun
from myutils.config import _TR, getlang_inner2show
from myutils.utils import dynamiclink
from ocrengines.baseocrclass import baseocr
from qtsymbols import *
from gui.dynalang import LPushButton, LLabel
from gui.dynalang import LPushButton, LFormLayout, LLabel
from gui.usefulwidget import SuperCombo, getboxlayout, IconButton
import threading, subprocess
from language import Languages


def getallsupports():
    _ = subprochiderun(
        "powershell Get-WindowsCapability -Online | Where-Object { $_.Name -Like 'Language.OCR*' }",
    ).stdout.splitlines()
    langs = []
    langsinter = []
    for i in range(len(_)):
        if _[i].startswith("Name"):
            if "Installed" in _[i + 1]:
                continue
            langs.append(re.search("Language.OCR~~~(.*?)~", _[i]).groups()[0])
            langsinter.append(re.search("Language.OCR~~~(.*)", _[i]).group())
    return langs, langsinter


class _SuperCombo(SuperCombo):
    setlist = pyqtSignal(list, list)

    def addItems(self, items, internals):
        self.clear()
        super().addItems(items, internals)


def loadlist(combo: _SuperCombo):
    lang, inter = getallsupports()
    combo.setlist.emit(lang, inter)


def installx(combo: _SuperCombo, btninstall, supportlang):
    if combo.currentIndex() == -1:
        return
    btninstall.setEnabled(False)
    combo.setEnabled(False)
    lang = combo.getIndexData(combo.currentIndex())
    subprocess.run(
        "powershell $Capability = Get-WindowsCapability -Online | Where-Object {{ $_.Name -Like '{}' }} | Add-WindowsCapability -Online".format(
            lang
        ),
    )
    lang, inter = getallsupports()
    combo.setlist.emit(lang, inter)
    btninstall.setEnabled(True)
    combo.setEnabled(True)
    supportlang.setText(", ".join([_[1] for _ in winrtutils.getlanguagelist()]))


def question():
    dialog = QWidget()
    formLayout = LFormLayout(dialog)
    formLayout.setContentsMargins(0, 0, 0, 0)
    supportlang = QLabel()
    supportlang.setText(", ".join([_[1] for _ in winrtutils.getlanguagelist()]))
    supportlang.setWordWrap(True)
    formLayout.addRow("当前支持的语言", supportlang)

    lst = []
    if not windows.IsUserAnAdmin():
        nopri = LLabel("权限不足，请以管理员权限运行！")
        lst.append(nopri)
        btndownload = LPushButton("添加语言包")
        btndownload.clicked.connect(
            lambda: os.startfile(
                dynamiclink("{docs_server}/{lang}/useapis/ocrapi.html#离线ocr")
            )
        )
        lst.append(btndownload)
    else:
        combo = _SuperCombo()
        combo.setlist.connect(combo.addItems)
        btninstall = LPushButton("添加")
        btninstall.clicked.connect(
            lambda: threading.Thread(
                target=installx, args=(combo, btninstall, supportlang)
            ).start()
        )
        lst.append(combo)
        lst.append(btninstall)
        threading.Thread(target=loadlist, args=(combo,)).start()
        btndownload = IconButton("fa.question")
        btndownload.clicked.connect(
            lambda: os.startfile(
                dynamiclink("{docs_server}/{lang}/useapis/ocrapi.html#离线ocr")
            )
        )
        lst.append(btndownload)
    formLayout.addRow(
        "添加语言包",
        getboxlayout(lst, makewidget=True, margin0=True),
    )
    return dialog


class OCR(baseocr):

    def getlanguagespace(self, lang: str):
        lang = lang.split("-")[0]
        try:
            return Languages.fromcode(lang).space
        except:
            return " "

    def langmap(self):
        return {Languages.Chinese: "zh-Hans", Languages.TradChinese: "zh-Hant"}

    def ocr(self, imagebinary):
        supports = [_[0] for _ in winrtutils.getlanguagelist()]
        if len(supports) == 0:

            raise Exception(_TR("无可用语言"))
        if self.srclang == Languages.Auto:
            if len(supports) == 1:
                uselang = supports[0]
            else:
                self.raise_cant_be_auto_lang()
        else:
            if not winrtutils.check_language_valid(self.srclang):
                raise Exception(
                    _TR(
                        "系统未安装“{currlang}”的OCR模型\n当前支持的语言：{langs}"
                    ).format(
                        currlang=_TR(getlang_inner2show(self.srclang_1)),
                        langs=", ".join([_[1] for _ in winrtutils.getlanguagelist()]),
                    )
                )
            uselang = self.srclang
        ret = winrtutils.OCR_f(imagebinary, uselang, self.getlanguagespace(uselang))
        boxs = [_[1:] for _ in ret]
        texts = [_[0] for _ in ret]
        return {"box": boxs, "text": texts}