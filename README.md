# Privacy-Preserving
これは論文「Privacy-Preserving Deep Learning via Additively Homomorphic
Encryption」を再現実装したリポジトリである。<br>
https://eprint.iacr.org/2017/715.pdf

## Homomorphic Encryption
論文のVI. Concrete Evaluations With an LWE-Based Encryptionの加法準同型暗号の実行時間を計測する。<br>
C++のコードのコンパイルにはboost multiprecision library(https://www.boost.org/doc/libs/1_85_0/libs/multiprecision/doc/html/index.html)が必要である。
まず、sample_A.py, sample_R.py, sample_S.py, sample_em.pyを実行してsample以下にテキストファイルを生成してから、lwe.outを実行することによって実行時間が測定できる。

### 離散ガウス分布
discrete_gaussian_distributionではPythonで離散ガウス分布のサンプリングを行う。<br>
discretegauss.pyが離散ガウス分布および一様分布のサンプリングを行うライブラリであり<br>、
https://github.com/IBM/discrete-gaussian-differential-privacy<br>
から取得した。<br>
sample.pyは上記のライブラリを使用してサンプリングを行うメソッドを記述したクラスを定義している。<br>
これを呼び出して、以下のファイルではそれぞれ必要な行列、ベクトルをサンプリングする。<br>
sample_A.py：Aを一様分布で初期化する<br>
sample_em.py：e_1, e_2, e_3を離散ガウス分布、mを一様分布で初期化する<br>
sample_R.py：Rを離散ガウス分布で初期化する<br>
sample_S.py：Sを離散ガウス分布で初期化する<br>
sample_test.pyはsample.pyが動作するかテストするためのものである。<br>
eの後についているa, bはLWEのAddにおいて、二種類の暗号文を生成するために使用する。

### LWE暗号
lwe.cppではC++で、LWE暗号を実装している。<br>
論文のアルゴリズムに基づいて、必要な行列やベクトルの配列を定義したあと、discrete_gaussian_distribution/sample/以下のtxtファイルからパラメータを読み込む。<br>
LWEのアルゴリズムに従って、KeyGen、Enc、Dec、Addの計算を行い、実行時間を表示する。<br>
入力した平文ベクトルmとEncで暗号化し、Decで復号したm_barをtxtファイルとしてHomomorphicEncryption/result以下に出力する。<br>
また、Addで暗号化したベクトルE(m)同士をたしたものをDecで復号したものと、平文mを2倍したベクトルについてもHomomorphicEncryption/result以下に出力する。<br>
result以下のm.txt、m_bar.txtをLinuxコマンドdiffなどで比較することで正しくEncおよびDecが行えているか確かめることができる。<br>
同様に、m_add.txtとm_add_bar.txtを比較することで、Addが正しく行えているか確かめることができる。<br>
P.txtは公開鍵の一部の行列を出力したものである。

## LeakGradient
論文のIII.Gradients Leak Informationにおける図を再現するためのソースコードである。<br>
leakimage.pyを実行することでLeakGradient/result以下にa.png, b.png, c.png, d.pngが生成される。<br>
これは、それぞれFig.3の(a)、(b)、(c)、(d)に対応している。

Tensorflowでニューラルネットワークの学習を行い、テストデータのうちの画像を1枚用いて勾配の計算を行う。<br>
(a)は画像をそのまま出力したもので、(b)は通常のニューラルネットワークにおいて勾配を求め出力したものである。<br>
(c)は正則化つきのニューラルネットワークを学習させ、勾配を計算したもので、(d)は(b)の勾配にノイズを加えたものである。<br>

## MLP
論文のVI.Concrete Evaluations With an LWE-Based EncryptionのMLPの実行時間を計測する。<br>
論文ではTensorflowのバージョン1.1.0を使用しているが、ここでは、最新のバージョン2.17.0を使用してシミュレーションを行う。<br>
mlp.pyを実行するとMLPのモデルの作成と訓練データの学習、テストデータに対する評価を行い、訓練にかかる時間を出力する。<br>
MNISTの訓練データ6×10^4枚とテストデータ10^4枚を使用する。<br>
入力層が784次元、中間層が64次元、出力層が10次元である。<br>
パラメータは平均0、標準偏差0.1の正規分布に従って初期化する。<br>
バッチサイズは50、エポックは17とした。<br>
