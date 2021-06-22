# DNA-simulation-with-Voxel
- 研究で使用したテストコードです。
- 研究テーマは、「voxelを用いたDNAのシミュレータ作成」です。
- 塩基をvoxel（立体の単位格子）に置き換えることで、高速なDNAのシミュレーションを試みています。
- この研究は、主に"Data generation"（データの生成）と"Simulation Accuracy Analysis"（シミュレーションの精度分析）に分かれています。
## Data generation（データの生成）
- DNAの塩基を置き換えるために使用するvoxelの設定と、シミュレーションによって得られるvoxelの座標のファイルへの書き出しを行います。
- 以下、[simulation.ipynb](https://github.com/nanami-7020/DNA-simulation-with-Voxel/blob/master/simulation.ipynb)の大まかな流れです。
1. モジュールのインストール。"vpython"を用いて、voxelを表示します。
2. voxelと一緒に表示するキャプションの用意。
3. あらかじめ用意していた[座標ファイル](https://github.com/nanami-7020/DNA-simulation-with-Voxel/blob/master/my_output.txt)の読み込み。  座標ファイルは、[Voxelyze](https://github.com/jonhiller/Voxelyze)を使用し、作成しました。  11個のvoxelに対し、10000timestepで10回実験を行った平均座標の結果を、記載しております。
4. ドット積の計算と可視化。
5. 持続長の定義。  持続長とは、溶液中におけるポリマー鎖の硬直具合と、変形する際にかかるエネルギーを計算するのに有効な指標です。  持続長が小さいほど、柔らかな素材で曲がり具合が大きいことを意味します。
6. 持続長の計算と、voxelの座標の修正。  修正前後によって、ドット積の計算結果がどのように変化するのかを可視化します。  変化の差が小さいほど、シミュレーション結果が正確であると判断します。
7. voxelの表示。
