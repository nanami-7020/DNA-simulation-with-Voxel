#include "Voxelyze.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "yaml-cpp/yaml.h"
#include <string>

void send_position(std::ostream&, std::vector<CVX_Voxel*>, int);
//std::ostream ストリームバッファオブジェクトに対する書き込み操作を提供する
//std::vector<型> <>型の動的配列クラス

void do_simulation(YAML::Node config, std::ostream&);

int main(int argc, char** argv){

  if(argc < 2){
    std::cout << "Usage: " << argv[0] << " config" << std::endl;
    std::exit(1);
  }

  YAML::Node config = YAML::LoadFile(argv[1]);

  std::string output = config["output_file"].IsDefined()?config["output_file"].as<std::string>():"position_test.txt";
  
  std::ofstream outputfile(output);
  for (int i=0; i<config["repeats"].as<int>(); i++){//10回実験を行う
    do_simulation(config, outputfile);
  }

  outputfile.close();
}

void send_position(std::ostream& stream, std::vector<CVX_Voxel*> voxels, int step){
  stream << step;
  for(int i =0; i< voxels.size(); i++){
    stream <<   " "  << voxels[i]->position().x << " " << voxels[i]->position().y << " " << voxels[i]->position().z;
      }
  
  stream  << std::endl;
}

void do_simulation(YAML::Node config, std::ostream& outputfile){
  std::random_device seed_gen;//擬似乱数の初期シードを生成（実行される度に同じ乱数が生成されるのを防ぐ）
  std::mt19937 engine(seed_gen());//擬似乱数を生成
  YAML::Node force = config["force"];
  std::uniform_real_distribution<> dist1(force["min_force"].as<double>(), force["max_force"].as<double>());//一様分布乱数を生成, dist(-1.0,1.0)より、-1.0~1.0の値が生成される。呼び出すときには、擬似乱数生成エンジンを渡す。
  CVoxelyze Vx(config["voxel_size"].as<double>()); //5mm voxels
  CVX_Material* pMaterial = Vx.addMaterial(config["pmaterial"]["stiffness"].as<int>(), 1000);// TODO //A material with stiffness E=1MPa and density 1000Kg/m^3

  std::vector<CVX_Voxel*> to_output;//std::vector 動的配列
  std::vector<CVX_Voxel*> average;//std::vector 動的配列
  //for(int i =0; i<11; i++){ //Create 11 voxels
  YAML::Node voxels = config["voxels"];

  for (auto i = voxels.begin(); i != voxels.end() ; ++i ){
    int x = i->second["x"].as<int>();
    int y = i->second["y"].IsDefined()?i->second["y"].as<int>():0;
    int z = i->second["z"].IsDefined()?i->second["z"].as<int>():0;
    
    //to_output.push_back(Vx.setVoxel(pMaterial, i, 0, 0)
    //to_output.push_back(Vx.setVoxel(pMaterial, voxels[i]["x"], 0, 0));
    to_output.push_back(Vx.setVoxel(pMaterial,x, y, z)); //Beam extends in the +X direction
    
  }//push_back 新たな要素を末尾に挿入する

  to_output[0]->external()->setFixedAll(); //Fixes all 6 degrees of freedom with an external condition on Voxel 1

  

  std::cout << Vx.recommendedTimeStep() << " ";//std::cout 標準出力


  for(int k=0; k<config["sim_length"].as<int>(); k++){//100000 timesteps
  
    Vx.doTimeStep();
    
    for(int i = 1; i<voxels.size(); i++){
      to_output[i]->external()->setForce(0,0,dist1(engine));
    }
    send_position(std::cout, to_output, k);//ターミナルに表示
    if (k%1000 == 0)
      send_position(outputfile, to_output, k);//ファイルに出力      
    }
  send_position(outputfile, to_output, 9999);
  

 
}
