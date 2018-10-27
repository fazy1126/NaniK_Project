#ifndef DEF_ECOMMAND_HPP
#define DEF_ECOMMAND_HPP

enum eCommand { 
  RestMyHp, //自分のHPがn以上m以下
  RestEnemyHp, //敵のHPがn以上m以下
  MyLastAction, //自分の直前の行動
  EnemyLastAction, //敵の直前の行動
  Random, //n/1の確率
  Else, //上記の条件に当てはまらなかった場合
};

#endif