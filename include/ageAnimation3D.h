//#######################################################################//
//
//						Animation3D, Animation3dKey
//	
//		The Animation3D structure is structure for storing one simple
//	animation of 3D object.
//		The Animation3dNode structure is a structure for storing one key
//	of animation.
// 
//		The reason why Animation3D structure haven't import methods for
//	import the same as Model3dData structure.
//
//
//#######################################################################//



#pragma once



namespace age {

	struct Animation3dKey {
		float time_val = 0.0f;
		float position[3];
		float rotation[3];
		float scaling[3];
	};

	struct Animation3D {
		~Animation3D();
		float anim_duration = 0.0f;
		int node_num = 0;
		Animation3dKey* nodes;
	};

}
