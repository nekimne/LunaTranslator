from qtsymbols import *
import os, functools
import gobject
from myutils.utils import splitocrtypes
from myutils.config import globalconfig
from gui.inputdialog import (
    autoinitdialog_items,
    noundictconfigdialog1,
    autoinitdialog,
    yuyinzhidingsetting,
)
from tts.basettsclass import TTSbase
from gui.setting_about import offlinelinks
from gui.setting_textinput import loadvalidtss
from gui.usefulwidget import (
    D_getsimplecombobox,
    D_getspinbox,
    makescrollgrid,
    D_getIconButton,
    yuitsu_switch,
    FocusCombo,
    D_getsimpleswitch,
    getboxlayout,
    getsmalllabel,
)


def showvoicelist(self, obj: TTSbase):

    if obj is None:
        try:
            self.voicecombo.clear()
        except:
            pass
        try:
            self.pitch____.setEnabled(False)
        except:
            pass
        try:
            self.rate____.setEnabled(False)
        except:
            pass
        return
    vl = obj.voiceshowlist
    idx = obj.voicelist.index(obj.voice)
    try:
        self.pitch____.setEnabled("pitch" not in obj.arg_not_sup)
    except:
        self.pitch____c = "pitch" not in obj.arg_not_sup
    try:
        self.rate____.setEnabled("rate" not in obj.arg_not_sup)
    except:
        self.rate____c = "rate" not in obj.arg_not_sup
    try:

        self.voicecombo.clear()
        self.voicecombo.addItems(vl)
        self.voicecombo.setCurrentIndex(idx)
    except:
        self.voicecombo_cache = vl, idx


def changevoice(self, _):
    if gobject.baseobject.reader is None:
        return
    gobject.baseobject.reader.voice = gobject.baseobject.reader.voicelist[
        self.voicecombo.currentIndex()
    ]


def createrate(self):
    self.rate____ = getboxlayout(
        [
            "语速_(-10~10)",
            D_getspinbox(
                -10,
                10,
                globalconfig["ttscommon"],
                "rate",
                step=0.5,
                double=True,
            ),
        ],
        margin0=True,
        makewidget=True,
    )
    try:
        self.rate____.setEnabled(self.rate____c)
    except:
        self.rate____.setEnabled(False)
    return self.rate____


def createpitch(self):
    self.pitch____ = getboxlayout(
        [
            "音高_(-10~10)",
            D_getspinbox(
                -10,
                10,
                globalconfig["ttscommon"],
                "pitch",
                step=0.5,
                double=True,
            ),
        ],
        margin0=True,
        makewidget=True,
    )
    try:
        self.pitch____.setEnabled(self.pitch____c)
    except:
        self.pitch____.setEnabled(False)
    return self.pitch____


def createvoicecombo(self):

    self.voicecombo = FocusCombo()
    self.voicecombo.currentTextChanged.connect(lambda x: changevoice(self, x))
    try:
        vl, idx = self.voicecombo_cache
        self.voicecombo.addItems(vl)
        if idx >= 0:
            self.voicecombo.setCurrentIndex(idx)
    except:
        pass
    return self.voicecombo


def setTab5(self, l):
    makescrollgrid(setTab5lz(self), l)


def getttsgrid(self, names):

    grids = []
    i = 0
    self.ttswitchs = {}
    line = []
    for name in names:

        _f = "./LunaTranslator/tts/{}.py".format(name)
        if os.path.exists(_f) == False:
            continue
        if "args" in globalconfig["reader"][name]:
            items = autoinitdialog_items(globalconfig["reader"][name])
            items[-1]["callback"] = functools.partial(
                gobject.baseobject.startreader, name, True, True
            )
            _3 = D_getIconButton(
                callback=functools.partial(
                    autoinitdialog,
                    self,
                    globalconfig["reader"][name]["args"],
                    globalconfig["reader"][name]["name"],
                    800,
                    items,
                    "tts." + name,
                    name,
                ),
                icon="fa.gear",
            )

        else:
            _3 = ""

        line += [
            globalconfig["reader"][name]["name"],
            D_getsimpleswitch(
                globalconfig["reader"][name],
                "use",
                name=name,
                parent=self,
                callback=functools.partial(
                    yuitsu_switch,
                    self,
                    globalconfig["reader"],
                    "ttswitchs",
                    name,
                    gobject.baseobject.startreader,
                ),
                pair="ttswitchs",
            ),
            _3,
        ]
        if i % 3 == 2:
            grids.append(line)
            line = []
        else:
            line += [""]
        i += 1
    if len(line):
        grids.append(line)
    return grids


def setTab5lz(self):
    grids = []
    offline, online = splitocrtypes(globalconfig["reader"])
    alltrans, alltransvis = loadvalidtss()
    offilesgrid = getttsgrid(self, offline)
    offilesgrid += [
        [(functools.partial(offlinelinks, "tts"), 0)],
    ]
    grids += [
        [
            (
                dict(
                    title="引擎",
                    type="grid",
                    grid=[
                        [
                            (
                                dict(
                                    title="离线",
                                    type="grid",
                                    grid=offilesgrid,
                                ),
                                0,
                                "group",
                            )
                        ],
                        [
                            (
                                dict(
                                    title="在线",
                                    type="grid",
                                    grid=getttsgrid(self, online),
                                ),
                                0,
                                "group",
                            )
                        ],
                    ],
                ),
                0,
                "group",
            )
        ],
        [
            (
                dict(
                    title="声音",
                    type="grid",
                    grid=[
                        [
                            getsmalllabel("选择声音"),
                            (functools.partial(createvoicecombo, self)),
                        ],
                        [
                            (
                                getboxlayout(
                                    [
                                        getboxlayout(
                                            [
                                                "音量_(0~100)",
                                                D_getspinbox(
                                                    0,
                                                    100,
                                                    globalconfig["ttscommon"],
                                                    "volume",
                                                ),
                                            ],
                                            margin0=True,
                                            makewidget=True,
                                        ),
                                        "",
                                        functools.partial(createrate, self),
                                        "",
                                        functools.partial(createpitch, self),
                                    ],
                                    margin0=True,
                                    makewidget=True,
                                ),
                                -1,
                            )
                        ],
                    ],
                ),
                0,
                "group",
            )
        ],
        [
            (
                dict(
                    title="行为",
                    type="grid",
                    grid=[
                        [
                            (
                                dict(
                                    type="grid",
                                    grid=[
                                        [
                                            "自动朗读",
                                            D_getsimpleswitch(
                                                globalconfig,
                                                "autoread",
                                                name="autoread",
                                                parent=self,
                                            ),
                                            "",
                                            "不被打断",
                                            D_getsimpleswitch(
                                                globalconfig, "ttsnointerrupt"
                                            ),
                                            "",
                                            "自动前进",
                                            D_getsimpleswitch(
                                                globalconfig, "ttsautoforward"
                                            ),
                                        ],
                                    ],
                                ),
                                0,
                                "group",
                            )
                        ],
                        [
                            (
                                dict(
                                    type="grid",
                                    grid=[
                                        [
                                            "朗读原文",
                                            D_getsimpleswitch(globalconfig, "read_raw"),
                                            "",
                                            "朗读翻译",
                                            D_getsimpleswitch(
                                                globalconfig, "read_trans"
                                            ),
                                            D_getsimplecombobox(
                                                alltransvis,
                                                globalconfig,
                                                "read_translator2",
                                                internal=alltrans,
                                            ),
                                            "",
                                        ],
                                    ],
                                ),
                                0,
                                "group",
                            )
                        ],
                        [
                            (
                                dict(
                                    type="grid",
                                    grid=[
                                        [
                                            "语音指定",
                                            D_getsimpleswitch(
                                                globalconfig["ttscommon"], "tts_skip"
                                            ),
                                            D_getIconButton(
                                                callback=lambda: yuyinzhidingsetting(
                                                    self,
                                                    globalconfig["ttscommon"][
                                                        "tts_skip_regex"
                                                    ],
                                                ),
                                                icon="fa.gear",
                                            ),
                                        ],
                                        [
                                            "语音修正",
                                            D_getsimpleswitch(
                                                globalconfig["ttscommon"], "tts_repair"
                                            ),
                                            D_getIconButton(
                                                callback=lambda: noundictconfigdialog1(
                                                    self,
                                                    globalconfig["ttscommon"][
                                                        "tts_repair_regex"
                                                    ],
                                                    "语音修正",
                                                    ["正则", "转义", "原文", "替换"],
                                                ),
                                                icon="fa.gear",
                                            ),
                                            "",
                                            D_getsimpleswitch(
                                                globalconfig["ttscommon"],
                                                "tts_repair_use_at_translate",
                                            ),
                                            "作用于翻译",
                                            "",
                                            "",
                                        ],
                                    ],
                                ),
                                0,
                                "group",
                            )
                        ],
                    ],
                ),
                0,
                "group",
            )
        ],
    ]
    return grids