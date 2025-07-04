# キャラクターごとに異なる声を使用した

まず、現在のテキストに名前やその他の識別情報が含まれていない��合、テキストセレクタで名前のテキストを追加で選択できます。表示されるテキストは選択の順序で並べられます。

次に、`ゲーム設定` -> `音声`（または設定インターフェースの`音声設定`、ただしこれはグローバル設定であり、グローバル設定には推奨されません）で`デフォルトに従う`を無効にし、`音声割り当て`を有効にして、その設定で行を追加します。`条件`を`含む`に設定し、`ターゲット`にキャラクターの名前を入力し、`割り当て先`で声を選択します。

![img](https://image.lunatranslator.org/zh/tts/1.png) 

ただし、名前のテキストが追加で選択されているため、表示および翻訳される内容には名前が含まれ、声も名前を読み上げます。この問題を解決するために、`音声修正`を有効にし、正規表現を使用して名前とその記号をフィルタリングできます。

## 音声割り当ての詳細な説明

現在のテキストが条件を満たすと、`割り当て先`で指定されたアクションが実行されます。

#### 条件

1. 正規表現
    判断に正規表現を使用するかどうか。
1. 条件
    **開始/終了** 開始/終了に設定すると、ターゲットがテキストの開���または終了にある場合にのみ条件が満たされます。
    **含む** ターゲットがテキストに現れる限り、条件が満たされます。これはより緩やかな判断です。
    `正規表現`が同時に有効になっている場合、正規表現はこのオプションに対応するように自動的に処理されます。
1. ターゲット
    判断に使用されるテキスト、通常は**キャラクターの名前**。
    `正規表現`が有効になっている場合、内容はより正確な判断のために正規表現として使用されます。

#### 割り当て先

1. スキップ
    条件が満たされると、現在のテキストの読み上げをスキップします。

1. デフォルト
    条件を満たす内容に対してデフォルトの声を使用します。通常、非常に緩やかな判断を使用する場合、誤検出が発生しやすいです。このアクションに設定された判断をより緩やかな判断の前に移動することで、誤検出を回避できます。
1. 声を選択
    選択後、声のエンジンと声を選択するウィンドウが表示されます。条件が満たされると、この声が読み上げに使用されます。
