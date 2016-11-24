

#include "lTechDemo.h"

unsigned int HEIGHTMAP_ELEMENTS_SIZE = 4;
unsigned int ELEMENTS_SIZE = 16;

class lSphereParticlefactory : public lParticleSystem::liParticleFactory
{
private:
	liBodyFactory			*BodyFactory;
    std::list<liAgent *>	&Agents;

    liResourceManager	*RM;
    liRenderer			*Renderer;

	//TMP!!!!!!!!!!!!!!!!!!!!
	static constexpr lmScalar Mass = 0.25;
	static constexpr lmScalar Radius = 0.2;
	//TMP!!!!!!!!!!!!!!!!!!!!

public:

	virtual liRigidBody *CreateParticle(const lmVector3D &position,const lmVector3D &velocity,const lmVector3D &orientation,const lmVector3D &angular_momentum) override
	{
		liRigidBody::liState *State = BodyFactory->CreateState(position,Mass*velocity,orientation,angular_momentum,true,true);
		//liRigidBody *Body = SimulationWorld->AddSphere(false,Mass,State,Radius);
		liRigidBody *Body = BodyFactory->AddParticle(false,Mass,State);

		liStaticMesh    *BallMesh   = RM->LoadStaticMesh("IdiotaAlakzatParticle.obj");
		liMultiMaterial *BallMtl    = RM->LoadMtl("IdiotaAlakzatParticle.mtl");

		//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		/*const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation,lrStaticMesh *mesh,lrMultiMaterial *material*/
		lrsStaticMeshNode *MeshNode = new lrsStaticMeshNode(State->GetPosition(),lmVector3D({Radius,Radius,Radius}),State->GetOrientationQuaternion(),BallMesh,BallMtl);
		MeshNode->Add(Renderer);

		lPhysicalObject_StaticMesh *Object = new lPhysicalObject_StaticMesh(State,MeshNode);
		Agents.push_back(Object);
		//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		return Body;
	}

	virtual liPointLight *CreateLight(float ambient[],float diffuse[],float specular[],float attenuation[]) override
	{
		return Renderer->CreatePointLight(ambient,diffuse,specular,attenuation,{0.0,0.0,0.0});
	}

	lSphereParticlefactory(liBodyFactory *body_factory,std::list<liAgent *> &agents,liResourceManager *rm,liRenderer *renderer)
		:BodyFactory(body_factory),Agents(agents),RM(rm),Renderer(renderer)
	{

	}

	virtual ~lSphereParticlefactory() override
	{

	}
};

void lTechDemo::AddBall()
{
	liStaticMesh    *BallMesh   = RM->LoadStaticMesh("Suzanne2.obj");
	liMultiMaterial *BallMtl    = RM->LoadMtl("Suzanne2.mtl");

	lmScalar Radius = 1.25;

	lmScalar Mlt;

	if(!BallConstraint)
	{
		Mlt = 5.0;
	}
	else
	{
		Mlt = 2.0;
	}

	liRigidBody::liState *BallState1 = SimulationWorld->GetBodyFactory()->CreateState(Camera->GetPosition() + 2.0*Camera->GetDirection(),DefaultMass*Mlt*Camera->GetDirection()/*5 m/s sebesség*/,{0.0,0.0,0.0},{0.0,0.0,0.0},true,true);
	liRigidBody *Sphere1 = SimulationWorld->GetBodyFactory()->AddSphere(false,DefaultMass,BallState1,Radius);
	/*
	lrsPhysSceneNode_StaticMesh *Obj = new lrsPhysSceneNode_StaticMesh(lmVector3D(Radius,Radius,Radius),BallState1,BallMesh,BallMtl);
	SimulatedSceneNodes.push_back(Obj);
	Obj->Add(Renderer);
	*/
	lrsStaticMeshNode *MeshNode1 = new lrsStaticMeshNode(BallState1->GetPosition(),lmVector3D({Radius,Radius,Radius}),BallState1->GetOrientationQuaternion(),BallMesh,BallMtl);
	MeshNode1->Add(Renderer);

	lPhysicalObject_StaticMesh *Object1 = new lPhysicalObject_StaticMesh(BallState1,MeshNode1);
	RunningAgents.push_back(Object1);


	if(BallConstraint)
	{
		//liRigidBody::liState *BallState2 = lpCreateState(Camera.GetPosition() + 4.0*Radius*Camera.GetDirection(),0.0*DefaultMass*5.0*Camera.GetDirection(),lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR);
		liRigidBody::liState *BallState2 = SimulationWorld->GetBodyFactory()->CreateState(Sphere1->GetPosition() + 4.0*Radius*lmVector3D({0.0,1.0,0.0}),0.0*DefaultMass*5.0*Camera->GetDirection(),{0.0,0.0,0.0},{0.0,0.0,0.0},true,true);
		liRigidBody *Sphere2 = SimulationWorld->GetBodyFactory()->AddSphere(false,0.0*DefaultMass,BallState2,Radius);
		/*
		lrsPhysSceneNode_StaticMesh *Obj = new lrsPhysSceneNode_StaticMesh(lmVector3D(Radius,Radius,Radius),BallState2,BallMesh,BallMtl);
		SimulatedSceneNodes.push_back(Obj);
		Obj->Add(Renderer);
		*/

		lrsStaticMeshNode *MeshNode2 = new lrsStaticMeshNode(BallState2->GetPosition(),lmVector3D({Radius,Radius,Radius}),BallState2->GetOrientationQuaternion(),BallMesh,BallMtl);
		MeshNode2->Add(Renderer);

		lPhysicalObject_StaticMesh *Object2 = new lPhysicalObject_StaticMesh(BallState2,MeshNode2);
		RunningAgents.push_back(Object2);

		lmVector3D P12 = Sphere2->GetPosition() - Sphere1->GetPosition();
		P12.Normalize();

		lmScalar c_Stiffness = 250;
		lmScalar c_DampingCoefficient = 50;

		SimulationWorld->GetBodyFactory()->AddSpring(Sphere1,Sphere2,1.0*P12 * Radius,-1.0 * P12 * Radius,0.25*Radius,c_Stiffness,c_DampingCoefficient);
		//Integrator->AddDistanceConstraint((lpRigidBody *)Sphere1,(lpRigidBody *)Sphere2,lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,2*Radius);
	}
}

void lTechDemo::AddMesh()
{
	//liStaticMesh    *Mesh   = RM->LoadStaticMesh("IdiotaAlakzat.obj");
	//liMultiMaterial *Mtl    = RM->LoadMtl("IdiotaAlakzat.mtl");

	liStaticMesh    *Mesh   = RM->LoadStaticMesh("barrel.obj");
	liMultiMaterial *Mtl    = RM->LoadMtl("Barrel.mtl");

	liRigidBody::liState *MeshState = SimulationWorld->GetBodyFactory()->CreateState(Camera->GetPosition() + 2.0*Camera->GetDirection(),DefaultMass*5.0*Camera->GetDirection()/*5 m/s sebesség*/,{0.0,0.0,0.0},{0.0,0.0,0.0},true,true);
	SimulationWorld->GetBodyFactory()->AddMesh(false,DefaultMass,MeshState,BodyMesh,true,false,ELEMENTS_SIZE);
	/*
	lrsPhysSceneNode_StaticMesh *Obj = new lrsPhysSceneNode_StaticMesh(lmVector3D(1.0,1.0,1.0),MeshState,Mesh,Mtl);
	SimulatedSceneNodes.push_back(Obj);
	Obj->Add(Renderer);
	*/
	lrsStaticMeshNode *MeshNode = new lrsStaticMeshNode(MeshState->GetPosition(),lmVector3D({1.0,1.0,1.0}),MeshState->GetOrientationQuaternion(),Mesh,Mtl);
	MeshNode->Add(Renderer);

	lPhysicalObject_StaticMesh *Object = new lPhysicalObject_StaticMesh(MeshState,MeshNode);
	RunningAgents.push_back(Object);
}

void lTechDemo::Logic()
{
	if(!GetPaused())
	{
		if(Input->GetUp())
		{
			Camera->MoveForward(0.2);
		}
		if(Input->GetDown())
		{
			Camera->MoveForward(-0.2);
		}
		if(Input->GetLeft())
		{
			Camera->SetYaw(Camera->GetYaw() - 0.02);
		}
		if(Input->GetRight())
		{
			Camera->SetYaw(Camera->GetYaw() + 0.02);
		}
		if(Input->GetRShift())
		{
			MeshOrBall = !MeshOrBall;
			SDL_Delay(200);
		}
		if(Input->GetLShift())
		{
			BallConstraint = !BallConstraint;
			SDL_Delay(200);
		}
		if(Input->GetEscape())
		{
			SetPaused(true);
			SDL_Delay(200);
			/*
			cout << "Vigyen be egy erteket:\n"
				 << "0: folytatas\n"
				 << "1: majmok tomegenek megvaltoztatasa\n"
				 << "2: majmok rugoallandojanak megvaltoztatasa\n"
				 << "3: kilepes\n"
				 << "egyeb: majmok csillapitasanak megvaltoztatasa\n";

			unsigned int MenuChoice;

			cin >> MenuChoice;

			//Csak az újonnan létrehozott majmok tulajdonságai változnak.
			switch(MenuChoice)
			{
			case 0:
				break;

			case 1:
				cin >> DefaultMass;
				break;

			case 2:
				//Olyan 5000-10000-es nagyságrendben néz ki jól. (50 kilo tömeg mellett)
				cin >> DefaultStiffness;
				break;
			case 3:
				SetNextMap("MainMenu");
				QuitGame();
				break;

			default:
				//Olyan 500-1000-es nagyságrendben néz ki jól. (50 kilo tömeg mellett)
				cin >> DefaultDampingCoefficient;
			}
			*/
		}

		if(Input->GetLmb())
		{
			if(MeshOrBall)
			{
				AddBall();
			}
			else
			{
				AddMesh();
			}

			SDL_Delay(200);
		}

		if(Input->GetW())
		{Player->MoveForward = true;}
		else
		{Player->MoveForward = false;}

		if(Input->GetS())
		{Player->MoveBackward = true;}
		else
		{Player->MoveBackward = false;}

		if(Input->GetA())
		{Player->MoveLeft = true;}
		else
		{Player->MoveLeft = false;}

		if(Input->GetD())
		{Player->MoveRight = true;}
		else
		{Player->MoveRight = false;}

		if(Input->GetW())
		{
			if(Anyad == false)
			{
				CharacterAnim->FireEvent("MoveForward");
				Anyad = true;
			}
		}
		else
		{
			if(Anyad == true)
			{
				CharacterAnim->FireEvent("StopMoving");
				Anyad = false;
			}
		}

		if(Input->GetS())
		{
			if(NagyAnyad == false)
			{
				CharacterAnim->FireEvent("MoveBackward");
				NagyAnyad = true;
			}
		}
		else
		{
			if(NagyAnyad == true)
			{
				CharacterAnim->FireEvent("StopMoving");
				NagyAnyad = false;
			}
		}

		if(Input->GetA())
		{
			if(NagyApad == false)
			{
				CharacterAnim->FireEvent("MoveLeft");
				NagyApad = true;
			}
		}
		else
		{
			if(NagyApad == true)
			{
				CharacterAnim->FireEvent("StopMoving");
				NagyApad = false;
			}
		}

		if(Input->GetD())
		{
			if(Apad == false)
			{
				CharacterAnim->FireEvent("MoveRight");
				Apad = true;
			}
		}
		else
		{
			if(Apad == true)
			{
				CharacterAnim->FireEvent("StopMoving");
				Apad = false;
			}
		}

		if(Input->GetQ())
		{Player->TurnLeft = true;}
		else
		{Player->TurnLeft = false;}

		if(Input->GetE())
		{Player->TurnRight = true;}
		else
		{Player->TurnRight = false;}
	}
	else
	{
		if(Input->GetEscape())
		{
			SetPaused(false);
			SDL_Delay(200);
		}
		if(Input->GetEnter())
		{
			SetNextMap("MainMenu");
			QuitGame();
			SDL_Delay(200);
		}
	}

	Player->Update(dt);
	CharacterAnim->Update(dt);

	/*
	t += dt;
	while(t > Animation->GetAnimDuration())
	{
		t -= Animation->GetAnimDuration();
	}
	*/

	//lrSkeleton Skeleton;
    //Animation->GetAnimation(t,Skeleton);
    CharacterAnim->GetSkeleton(&SkeletalMeshNode->GetSkeleton());

    //SkeletalMeshNode->SetSkeleton(Skeleton);
    SkeletalMeshNode->SetPosition(Player->GetPosition() - lmVector3D({0.0,PLAYER_RADIUS/2.0,0.0}));

    lmScalar cos = lmDot(lmVector3D({0.0,0.0,1.0}),Player->GetDirection());
    lmScalar sin = lmDot(lmVector3D({1.0,0.0,0.0}),Player->GetDirection());

    lmScalar Angle = acos(cos);
    if(sin < 0.0)
	{
		Angle *= -1,0;
	}

    lmQuaternion q = lmQuaternion(lmVector3D({0.0,1.0,0.0}),Angle);
    SkeletalMeshNode->SetOrientation(q);
}

lTechDemo::lTechDemo(liApiAdapter *api_adapter,std::string &next_map)
	:lGame(api_adapter,next_map)
{
	DefaultMass = 50.0;

	BgColor.Red		= 0.0;
	BgColor.Green	= 0.0;
	BgColor.Blue	= 0.0;

	//A szimulációnkat vegye körül 1 doboz.
	/*
	lrStaticMesh    *FalMesh    = RM->LoadStaticMesh("untitled.obj");
	lrMultiMaterial *FalMtl     = RM->LoadMtl("untitled.mtl");

	lrmPhMesh *PhMesh;
	lrmPhMesh::LoadWavefrontMesh("Content/Meshes/PhysMeshes/untitled.obj",&PhMesh);

	lpRigidBody::lpState *FalState = new lpRigidBody::lpState(lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR);
	//lpRigidBody::lpState *FalState = new lpRigidBody::lpState(lmVector3D(0.0,-1.5,0.0),lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR);
	SimulationWorld->AddMesh(0.0,20000.0,1000.0,FalState,PhMesh);
	//SimulationWorld->AddPlane(0.0,50000.0,1000.0,FalState,lmVector3D(0.0,1.0,0.0));

	delete PhMesh;

	FalObj = new lrsStaticMeshNode(lmVector3D::NULL_VECTOR,lmVector3D(8.5,1.5,8.5),lmQuaternion(0.0,0.0,0.0,1.0),FalMesh,FalMtl);
	FalObj->Add(Renderer);
	*/

    Character = RM->LoadCharacter("bob_lamp_update/bob_lamp_update_export.md5mesh");

    //SDL_Delay(30000);

    //Animation = RM->LoadSkeletalAnimation("bob_lamp_update/standing.md5anim");
    liMultiMaterial *MultiMaterial = RM->LoadMtl("bob_lamp_update/bob_lamp_update_export.md5mesh");

    liKeyframeAnimation *KeyframeAnims[4];

    KeyframeAnims[0] = RM->LoadSkeletalAnimation("bob_lamp_update/standing.md5anim");
    KeyframeAnims[1] = RM->LoadSkeletalAnimation("bob_lamp_update/step_forward.md5anim");
    KeyframeAnims[2] = RM->LoadSkeletalAnimation("bob_lamp_update/moving_forward.md5anim");
    KeyframeAnims[3] = RM->LoadSkeletalAnimation("bob_lamp_update/AnimCollection.md5anim");

    //dt = 1.0/180.0;

    /*
    KeyframeAnims[0] = RM->LoadSkeletalAnimation("bob_lamp_update/moving_forward.md5anim");
    KeyframeAnims[1] = RM->LoadSkeletalAnimation("bob_lamp_update/standing.md5anim");
    */

    CharacterAnim = new lrCharacterAnim(KeyframeAnims);
    /*
    MoveForwardEvent	= KeyframeAnim->GetPublicEvent("MoveForward");
	MoveBackwardEvent	= KeyframeAnim->GetPublicEvent("MoveBackward");
	MoveLeftEvent		= KeyframeAnim->GetPublicEvent("MoveLeft");
	MoveRightEvent		= KeyframeAnim->GetPublicEvent("MoveRight");
	StopMovingEvent		= KeyframeAnim->GetPublicEvent("StopMoving");
	*/
    lrSkeleton Skeleton;
    //Animation->GetAnimation(0.0,Skeleton);
    CharacterAnim->GetSkeleton(&Skeleton);

    SkeletalMeshNode = new lrsSkeletalMeshNode(lmVector3D({0.0,-4.0,-12.0}),lmVector3D({1.0,1.0,1.0}),lmQuaternion(0.0,0.0,0.0,1.0),Character,MultiMaterial,Skeleton);
    SkeletalMeshNode->Add(Renderer);

	//liStaticMesh    *HeightmapMesh	= RM->LoadHeightmap("Heightmap.bmp");
	liStaticMesh    *HeightmapMesh	= RM->LoadHeightmap("Untitled.bmp");
	//TMP!!!!!!!!!!!!!!!!!!!!!!!
	liStaticMesh    *FalMesh		= RM->LoadStaticMesh("untitled.obj");
	//TMP!!!!!!!!!!!!!!!!!!!!!!!
	liMultiMaterial *HeightmapMtl	= RM->LoadMtl("untitled.mtl");
	 /*
	float TileX = 20.0;
	float TileY = 8.0;
	float TileZ = 20.0;
	// */

	float TileX = 200.0;
	float TileY = 20.0;
	float TileZ = 200.0;
	// */

	//lpRigidBody::lpState *PlaneState = new lpRigidBody::lpState(lmVector3D(0.0,-16.0,0.0),lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,false,false);
	//liRigidBody *Plane = SimulationWorld->AddPlane(false,0.0,PlaneState,lmVector3D(0.0,1.0,0.0));

	//liPhMesh *PhMesh = RM->LoadPhysHeightmap("Content/Images/Heightmap/Heightmap.bmp",TileX,TileY,TileZ);
	liPhMesh *PhMesh = RM->LoadPhysHeightmap("Content/Images/Heightmap/Untitled.bmp",TileX,TileY,TileZ); //new lrmPhHeightmapMesh("Content/Images/Heightmap/Untitled.bmp",TileX,TileY,TileZ);

	liRigidBody::liState *HeightmapState = SimulationWorld->GetBodyFactory()->CreateState(lmVector3D({0.0,-10.0,0.0}),{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},true,true);
	SimulationWorld->GetBodyFactory()->AddMesh(false,0.0,HeightmapState,PhMesh,false,true,HEIGHTMAP_ELEMENTS_SIZE);

	FalObj = new lrsStaticMeshNode(lmVector3D({0.0,-10.0,0.0}),lmVector3D({TileX,TileY,TileZ}),lmQuaternion(0.0,0.0,0.0,1.0),HeightmapMesh,HeightmapMtl);
	FalObj->Add(Renderer);

	delete PhMesh;

	//BodyMesh = RM->LoadPhysMesh("Content/Meshes/PhysMeshes/IdiotaAlakzat.obj",false); //new lrmPhWavefrontMesh("Content/Meshes/PhysMeshes/IdiotaAlakzat.obj",false);


	BodyMesh = RM->LoadPhysMesh("Content/Meshes/PhysMeshes/barrel.obj",false); //new lrmPhWavefrontMesh("Content/Meshes/PhysMeshes/IdiotaAlakzat.obj",false);

	float particleAmbient[3] = {0.0,0.0,0.0};
    float particleDiffuse[3] = {0.6,1.0,0.2};
    //float particleDiffuse[3] = {1.0,1.0,1.0};
    float particleSpecular[3] = {0.0,0.25,0.0};

    float particleAttenuation[3] = {1.0,0.75,0.75};

    unsigned int NumParticles = /*25*3*5000;/*/64;
    float SpawnDelay = 0.0125;

    bool ParticleSysHasLight = true;

	lParticleSystem *ParticleSystem = new lParticleSystem(new lSphereParticlefactory(SimulationWorld->GetBodyFactory(),RunningAgents,RM,Renderer),NumParticles,false,lmVector3D({0.0,2.0,0.0}),10.0,lmVector3D({0.0,1.0,0.0}),3.0,
										 true,PI/4.0,false,3.5,
										 ParticleSysHasLight,particleAmbient,particleDiffuse,particleSpecular,particleAttenuation,
										 SpawnDelay);

	RunningAgents.push_back(ParticleSystem);

	float lAmbient[3] = {0.35,0.35,0.35};
    float lDiffuse[3] = {0.95,0.95,0.95};
    float lSpecular[3] = {0.75,0.75,0.75};

    float lAttenuation[3] = {1.0,0.0,0.0};

    lmVector3D lPosition({0.0,2.0,0.0});

    Renderer->CreatePointLight(lAmbient,lDiffuse,lSpecular,lAttenuation,lPosition);

	liRigidBody::liState *BallState	= SimulationWorld->GetBodyFactory()->CreateState(lmVector3D({0.0,0.0,-10.0}),{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},true,false);
	liRigidBody			 *Sphere	= SimulationWorld->GetBodyFactory()->AddSphere(false,DefaultMass,BallState,PLAYER_RADIUS);

	Player = new lPlayer(Sphere,PLAYER_RADIUS);

	//dt = 1.0/360.0;
}

lTechDemo::~lTechDemo()
{
	delete FalObj;
	delete BodyMesh;

	//delete ParticleSystem;
	delete Player;
	delete CharacterAnim;

	delete SkeletalMeshNode;
}
