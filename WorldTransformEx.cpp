#include<WorldTransform.h>
#include<math.h>

//スケール、回転、平行移動を合成して行列を計算する
void WorldTransform::UpdateMatrix(){ 
	matWorld_ = MakeAffineMatrix(scale_,rotation_ , translation_); 
	//定数バッファに転送する
	TransferMatrix();
};
