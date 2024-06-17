## だうぉっち　Da-watch
Chat GPTにて制作した、M5stack上で動作するペット育成シミュレーションゲームです。
好きな画像に差し替えたりシステムに改変を加えたりして楽しんでください。

## 機能

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
- **ソフトウェア**: 
  - Arduino IDE
  - 必要なライブラリ: `M5Core2`, `JPEGDecoder`, `NTPClient`

## インストール

1. **リポジトリをクローン**:
   ```bash
   git clone https://github.com/yourusername/PetSimulation.git
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
- Status Display: 現在の健康、幸福度、空腹度、好感度を表示します。
- Feed: 空腹度を20減少させ、幸福度を10増加させ、好感度を5増加させます。
- Play: 幸福度を20増加させ、好感度を10増加させます。
- Exercise: 健康を10増加させ、幸福度を10増加させます。
- Bath: 健康を10増加させ、幸福度を20増加させます。
- 時間に基づく動作
- 00:00から08:00の間、sleep.jpgが表示されます。
- 時刻とバッテリー残量は毎分更新されます。
