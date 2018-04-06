//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Character.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx/PhysxManager.h"

Character::Character(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(9.81f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.8f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0)
{
}


Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	//UNREFERENCED_PARAMETER(gameContext);
	// Create controller
	m_pController = new ControllerComponent(nullptr);
	AddComponent(m_pController);
	// Add a fixed camera as child
	auto fixedCamera = new FixedCamera();
	AddChild(fixedCamera);
	// Register all Input Actions
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::FORWARD, Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::BACKWARD, Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::RIGHT, Down, 'D'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::LEFT, Down, 'A'));
	gameContext.pInput->AddInputAction(InputAction(CharacterMovement::JUMP, Down, VK_SPACE));
}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Set the camera as active
	m_pCamera->SetActive();

	// We need to do this in the PostInitialize because child game objects only get initialized 
	//after the Initialize of the current object finishes

}


void Character::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Update the character (Camera rotation, Character Movement, Character Gravity)
	if (m_pCamera->IsActive())
	{
		XMFLOAT2 move = XMFLOAT2(0, 0);
		move.y = gameContext.pInput->IsActionTriggered(FORWARD) ? 1.0f : 0.0f;

		if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(BACKWARD)) ? 1.0f : 0.0f;
		if (move.y == 0) move.y = gameContext.pInput->GetThumbstickPosition().y;

		move.x = gameContext.pInput->IsActionTriggered(RIGHT) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(RIGHT)) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = gameContext.pInput->GetThumbstickPosition().x;

		XMFLOAT2 look = XMFLOAT2(0, 0);

		auto mouseMove = gameContext.pInput->GetMouseMovement();
		look.x = static_cast<float>(mouseMove.x);
		look.y = static_cast<float>(mouseMove.y);

		if (look.x == 0.0f && look.y == 0.0f)
		{
			look = gameContext.pInput->GetThumbstickPosition(false);
		}

		//CALCULATE TRANSFORMS
		XMVECTOR forward = XMLoadFloat3(&GetTransform()->GetForward());
		XMVECTOR right = XMLoadFloat3(&GetTransform()->GetRight());


		m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();

		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);

		m_RunVelocity = m_RunAcceleration * gameContext.pGameTime->GetElapsed();
		if (m_RunVelocity > m_MaxRunVelocity) m_RunVelocity = m_MaxRunVelocity;
		move.x *= m_RunVelocity;
		move.y *= m_RunVelocity;

		m_Velocity.x = move.x;
		m_Velocity.y = move.y;

		m_pController->Move(XMFLOAT3(m_Velocity.x* gameContext.pGameTime->GetElapsed(),
			m_Velocity.y * gameContext.pGameTime->GetElapsed(),
			m_Velocity.z * gameContext.pGameTime->GetElapsed()));
	}

}