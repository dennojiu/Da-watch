## だうぉっち　Da-watch
Chat GPTにて制作した、M5stack上で動作するペット育成シミュレーションゲームです。
好きな画像に差し替えたりシステムに改変を加えたりして楽しんでください。

## 機能

- **STATES DISPLAY**: 現在の健康、幸福度、空腹度、好感度を表示します。
- **FEED**: ペットに餌をあげる。空腹度を減少させ、幸福度と好感度を増加させます。
- **PLAY**: ペットと遊ぶ。幸福度と好感度を増加させます。
- **EXERCISE**: ペットを運動させる。健康と幸福度を増加させます。
- **BATH**: ペットを入浴させる。健康と幸福度を増加させます。
- **スリープモード**: 00:00から08:00の間、ペットはスリープモードに入り、睡眠画像が表示されます。
- **時計とバッテリー表示**: 画面上部に時刻とバッテリー残量を表示します。
- **自動保存**: 5分ごとにペットの状態をSDカードに自動保存します。

## Requirements / 必要条件

- **ハードウェア**: 
  - M5Stack Core2
  - 以下の画像ファイルが含まれたSDカード:
    - `background.jpg`
    - `feed1.jpg`, `feed2.jpg`, `feed3.jpg`
    - `play1.jpg`, `play2.jpg`, `play3.jpg`
    - `exercise1.jpg`, `exercise2.jpg`, `exercise3.jpg`
    - `bath.jpg`
    - `appearance1.jpg`, `appearance2.jpg`, `appearance3.jpg`, `appearance4.jpg`, `appearance5.jpg`, `appearance6.jpg`
    - `sleep.jpg`
   imgフォルダにサンプル画像が入っていますので自由にお使いください。

- **ソフトウェア**: 
  - Arduino IDE
  - 必要なライブラリ: `M5Core2`, `JPEGDecoder`, `NTPClient`

## インストール

1. **リポジトリをクローン**:
   ```bash
   [git clone (https://github.com/dennojiu/Da-watch)
2. Arduino IDEでプロジェクトを開く。
3. コードをM5Stack Core2にアップロード。
4. SDカードを準備:
   必要な画像ファイルをSDカードのルートディレクトリにコピーします。
6. SDカードをM5Stack Core2に挿入します。

## 操作
  - ボタンA: コマンドを選択（ステータス表示、餌をあげる、遊ぶ、運動、入浴の順に循環）。
  - ボタンB: 選択したコマンドを実行。
  - ボタンC: 現在のコマンドをキャンセルし、ステータス表示に戻ります。
 
## コマンドと効果
- Status Display: 現在の健康、幸福度、空腹度、好感度を表示します。健康、幸福度、好感度は1時間ごとに5ずつ減少し、空腹度は5ずつ増加します。各ステータスは、0以下または100以上にならないように制御されています。
- Feed: 空腹度を20減少させ、幸福度を10増加させ、好感度を5増加させます。`feed1.jpg`, `feed2.jpg`, `feed3.jpg`がランダムに表示されます。
- Play: 幸福度を20増加させ、好感度を10増加させます。`play1.jpg`, `play2.jpg`, `play3.jpg`がランダムに表示されます。
- Exercise: 健康を10増加させ、幸福度を10増加させます。`exercise1.jpg`, `exercise2.jpg`, `exercise3.jpg`がランダムに表示されます。
- Bath: 健康を10増加させ、幸福度を20増加させます。`bath.jpg`が表示されます。
- 時間に基づく動作
- 00:00から08:00の間、sleep.jpgが表示されます。
- 時刻とバッテリー残量は毎分更新されます。

## アピアランス画像表示の条件
- APPEARANCE1:
デフォルトのアピアランス。
その他の条件に該当しない場合に表示されます。
- APPEARANCE2:
幸福度が50以下の場合に表示されます。
- APPEARANCE3:
空腹度が70以上の場合に表示されます。
- APPEARANCE4:
健康が50以下の場合に表示されます。
- APPEARANCE5:
好感度が50以下の場合に表示されます。
- APPEARANCE6:
好感度が100の場合に表示されます。

