#ifndef COMETA_COMPONENTS_H
#define COMETA_COMPONENTS_H

#include <iostream>
#include <string>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>

#include "render/Texture.h"
#include "render/Mesh.h"
#include "render/Material.h"

#include "physics/Collider.h"
class Collision;

class Entity;
class Renderer;


#include "world/BaseScript.h"

/**
 * Component virtual class
 */
class Component {
protected:
	Entity* _owner = nullptr;

public:
	virtual ~Component() = default;

	virtual void Init() = 0;

	// ------------ GETTERS AND SETTERS ------------
	[[nodiscard]] Entity* GetOwner() const { return _owner; }
	void SetOwner(Entity* newOwner) { _owner = newOwner; }

	friend class Entity;
	friend class ComponentRegistry;
};

class Transform : public Component {
public: 

	glm::vec3 position = { 0.0f, 0.0f, 0.0f }; 
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

private:
	Transform* _parent = nullptr;

public:
	Transform() = default;

	explicit Transform(const glm::vec3& position)
	{
		this->position = position;
		this->rotation = { 0.0f, 0.0f, 0.0f };
		this->scale = { 1.0f, 1.0f, 1.0f };
	}

	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : position(position), rotation(rotation), scale(scale) {};

	Transform(const Transform& other)
	{
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
	};

	void Init() override {}

	[[nodiscard]] glm::mat4 GetTransform() const {
		const glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(this->rotation)));
		return glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
	}

	[[nodiscard]] glm::mat4 GetWorldTransform() const{
		if (_parent != nullptr){
			return _parent->GetWorldTransform() * GetTransform();
		}
		else{
			return GetTransform();
		}
	}

	void SetParent(Transform* newParent){_parent = newParent;}
	[[nodiscard]] Transform* GetParent() const { return _parent; }

	friend std::ostream& operator<<(std::ostream& os, const Transform& transform)
	{
		os << "  - Transform: "
						  << "Pos(" << transform.position.x << ", "
						  << transform.position.y << ", "
						  << transform.position.z << "), "
						  << "Rot(" << transform.rotation.x << ", "
						  << transform.rotation.y << ", "
						  << transform.rotation.z << "), "
						  << "Scale(" << transform.scale.x << ", "
						  << transform.scale.y << ", "
						  << transform.scale.z << ")" << std::endl;
		return os;
	}
};

class MeshRenderable : public Component {
private:
	std::shared_ptr<Mesh> _mesh = nullptr;
	std::shared_ptr<Material> _material = nullptr;

public:
	MeshRenderable() = default;
	MeshRenderable(const MeshRenderable&) = default;

	void Init() override {}

	// Properties management methods

	void SetMesh(const std::shared_ptr<Mesh>& mesh) { _mesh = mesh; }
	void SetMaterial(const std::shared_ptr<Material>& material) {_material = material;}

	// ------------ GETTERS ------------
	[[nodiscard]] std::shared_ptr<Mesh> GetMesh() const { return _mesh; }
	[[nodiscard]] std::shared_ptr<Material> GetMaterial() const { return _material; }
};


class SpriteRenderable : public Component {
public: 
	Texture texture; 
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	void Init() override {}

	SpriteRenderable() = default;
	SpriteRenderable(const SpriteRenderable&) = default;
};


// ----------------------------------------------
// |            LIGHTNING COMPONENTS            |
// ----------------------------------------------

class PointLight : public Component{
	friend class Renderer;
private:
	glm::vec3 _ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 _diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	glm::vec3 _specular = glm::vec3(1.0f, 1.0f, 1.0f);

	float _constant = 1.0f;
	float _linear = 0.07f;
	float _quadratic = 0.017f;

public:
	PointLight() = default;
	PointLight(const PointLight&) = default;

	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic):
		_ambient(ambient), _diffuse(diffuse), _specular(specular), _constant(constant), _linear(linear), _quadratic(quadratic) {};

	void Init() override {}

	// ------ GETTERS ---------
	[[nodiscard]] glm::vec3 GetAmbient() const { return _ambient; }
	[[nodiscard]] glm::vec3 GetDiffuse() const { return _diffuse; }
	[[nodiscard]] glm::vec3 GetSpecular() const { return _specular; }
	[[nodiscard]] float GetConstant() const { return _constant; }
	[[nodiscard]] float GetLinear() const { return _linear; }
	[[nodiscard]] float GetQuadratic() const { return _quadratic; }

	// ------ SETTERS ------
	void SetAmbient(const glm::vec3& ambient) { _ambient = ambient; }
	void SetDiffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular) { _specular = specular; }
	void SetConstant(float constant) { _constant = constant; }
	void SetLinear(float linear) { _linear = linear; }
	void SetQuadratic(float quadratic) { _quadratic = quadratic; }
};

class DirectionalLight : public Component
{
	friend class Renderer;
private:
	glm::vec3 _direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 _ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 _diffuse = glm::vec3(0.35f, 0.4f, 0.35f);
	glm::vec3 _specular = glm::vec3(0.5f, 0.5f, 0.5f);
	
	
	// Shadow mapping properties
	float _shadowNearPlane = 1.0f;
	float _shadowFarPlane = 25.0f;
	float _shadowOrthoSize = 10.0f;

	// Cached light space matrix
	glm::mat4 _lightSpaceMatrix = glm::mat4(1.0f);
	bool _lightSpaceMatrixDirty = true;

	// Recalculate the light space matrix
	void UpdateLightSpaceMatrix() {
		// Create light projection matrix (orthographic for directional light)
		glm::mat4 lightProjection = glm::ortho(
			-_shadowOrthoSize, _shadowOrthoSize,
			-_shadowOrthoSize, _shadowOrthoSize,
			_shadowNearPlane, _shadowFarPlane);

		// Create light view matrix
		glm::vec3 lightPos = -_direction * 10.0f; // Position light far away in the opposite direction
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Calculate light space matrix
		_lightSpaceMatrix = lightProjection * lightView;
		_lightSpaceMatrixDirty = false;
	}

public:
	DirectionalLight() = default;
	DirectionalLight(const DirectionalLight&) = default;

	void Init() override {}

	// --------- GETTERS ---------
	[[nodiscard]] glm::vec3 GetDirection() const { return _direction; }
	[[nodiscard]] glm::vec3 GetAmbient() const { return _ambient; }
	[[nodiscard]] glm::vec3 GetDiffuse() const { return _diffuse; }
	[[nodiscard]] glm::vec3 GetSpecular() const { return _specular; }
	
	// Shadow mapping getters
	[[nodiscard]] float GetShadowNearPlane() const { return _shadowNearPlane; }
	[[nodiscard]] float GetShadowFarPlane() const { return _shadowFarPlane; }
	[[nodiscard]] float GetShadowOrthoSize() const { return _shadowOrthoSize; }
	
	[[nodiscard]] glm::mat4 GetLightSpaceMatrix() {
		if (_lightSpaceMatrixDirty) {
			UpdateLightSpaceMatrix();
		}
		return _lightSpaceMatrix;
	}

	// --------- SETTERS ---------
	void SetDirection(const glm::vec3& direction) { 
		_direction = direction; 
		_lightSpaceMatrixDirty = true;
	}
	
	void SetAmbient(const glm::vec3& ambient) { _ambient = ambient; }
	void SetDiffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular) { _specular = specular; }
	
	// Shadow mapping setters
	void SetShadowNearPlane(float nearPlane) { 
		_shadowNearPlane = nearPlane; 
		_lightSpaceMatrixDirty = true;
	}
	
	void SetShadowFarPlane(float farPlane) { 
		_shadowFarPlane = farPlane; 
		_lightSpaceMatrixDirty = true;
	}
	
	void SetShadowOrthoSize(float size) { 
		_shadowOrthoSize = size; 
		_lightSpaceMatrixDirty = true;
	}
};


// ----------------------------------------------
// |              PHYSICS COMPONENTS            |
// ----------------------------------------------


class ColliderComponent : public Component {
protected:
	Collider*  _collider = nullptr;
	bool _isTrigger = false;
public:
	ColliderComponent() = default;
	ColliderComponent(const ColliderComponent& other) = default;

	~ColliderComponent() override = default;

	bool operator==(const ColliderComponent& other) const
	{
		if (_isTrigger != other._isTrigger)
			return false;

		if (_collider == nullptr && other._collider == nullptr)
			return true;

		if ((_collider == nullptr) != (other._collider == nullptr))
			return false;

		if (_collider->GetType() != other._collider->GetType())
			return false;

		if (_collider != other._collider){
			return false;
		}

		return true;
	}

	void Init() override {}

	template<typename T, typename... Args>
	T* SetCollider(Args&&... args) {
		static_assert(std::is_base_of<Collider, T>::value, "T must derive from Collider");
		_collider = new T(std::forward<Args>(args)...);
		return dynamic_cast<T*>(_collider);
	}

	[[nodiscard]] Collider* GetCollider() const { return _collider; }
	[[nodiscard]] bool IsTrigger() const { return _isTrigger; }
	void SetTrigger(bool isTrigger) { _isTrigger = isTrigger; }
};



class RigidBody : public Component
{
	friend class PhysicsManager;

private:
	// Linear
	glm::vec3 _linearVelocity = { 0.0f, 0.0f, 0.0f };
	glm::vec3 _force = { 0.0f, 0.0f, 0.0f };
	float _mass = 1.0f;

	// Angular
	glm::vec3 _torque = { 0.0f, 0.0f, 0.0f };
	glm::vec3 _angularVelocity = { 0.0f, 0.0f, 0.0f };
	glm::mat3 _inertiaTensor = glm::mat3();
	glm::mat3 _inverseInertiaTensor = glm::mat3();

	bool _enabled = true;
	bool _affectedByGravity = true;

public:
	RigidBody(){
		// Linear
		_linearVelocity = { 0.0f, 0.0f, 0.0f };
		_force = { 0.0f, 0.0f, 0.0f };
		_mass = 1.0f;

		_enabled = true;

		// Angular
		_torque = { 0.0f, 0.0f, 0.0f };
		_angularVelocity = { 0.0f, 0.0f, 0.0f };
		_inertiaTensor = glm::mat3();
		_inverseInertiaTensor = glm::mat3();
	}

	RigidBody(const RigidBody&) = default;

	void Init() override;
	void Reset();

	[[nodiscard]] float GetMass() const { return _mass; }
	[[nodiscard]] glm::vec3 GetLinearVelocity() const { return _linearVelocity; }
	[[nodiscard]] glm::vec3 GetAngularVelocity() const { return _angularVelocity; }
	[[nodiscard]] glm::vec3 GetForce() const { return _force; }
	[[nodiscard]] glm::vec3 GetTorque() const { return _torque; }
	[[nodiscard]] glm::mat3 GetInertiaTensor() const { return _inertiaTensor; }
	[[nodiscard]] glm::mat3 GetInverseInertiaTensor() const { return _inverseInertiaTensor; }

	[[nodiscard]] bool IsEnabled() const { return _enabled; }
	[[nodiscard]] bool& GetEnabledRef() { return _enabled; }
	void SetEnabled(bool isEnabled) { _enabled = isEnabled; }

	[[nodiscard]] bool IsAffectedByGravity() const { return _affectedByGravity; }
	void SetAffectedByGravity(bool value) {_affectedByGravity = value;}

	void SetMass(float mass) { _mass = mass; Init(); }
	void SetLinearVelocity(const glm::vec3& linearVelocity) { _linearVelocity = linearVelocity; }
	void SetAngularVelocity(const glm::vec3& angularVelocity) { _angularVelocity = angularVelocity; }
	void SetForce(const glm::vec3& force) { _force = force; }
	void SetTorque(const glm::vec3& torque) { _torque = torque; }

	void SetInertiaTensor(const glm::mat3& inertiaTensor){
		_inertiaTensor = inertiaTensor;
		_inverseInertiaTensor = glm::inverse(_inertiaTensor);
	}
};



class Tag : public Component {
private: 
	std::string _tag = ""; 
public: 
	Tag() = default;
	Tag(const Tag&) = default;
	explicit Tag(std::string  tag) : _tag(std::move(tag)) {}

	void Init() override {}

	std::string GetTag() { return _tag;  }
	void SetTag(const std::string& tag) { _tag = tag; }
};



class Script : public Component {
private:
	std::shared_ptr<BaseScript> _script = nullptr;

public:
	Script() = default;
	Script(const Script&) = default;

	// Move constructor
	// Script(Script&& other)
	// 	: Component(std::move(other)),  // Don't forget to move the base class
	// 	  _script(std::move(other._script))
	// {}
	//
	// // Move assignment operator
	// Script& operator=(Script&& other) noexcept {
	// 	if (this != &other) {
	// 		Component::operator=(std::move(other));  // Move base class
	// 		_script = std::move(other._script);
	// 	}
	// 	return *this;
	// }

	void Init() override {}
	/**
	 * Create and attach a script of the T type with the args passed to the constructor
	 * @tparam T Type of script to attach
	 * @tparam Args typename for Arguments
	 * @param args Arguments of the constructor of the script to attach
	 */
	template<typename T, typename... Args>
	void Attach(Args&&... args){
		_script = std::make_shared<T>(std::forward<Args>(args)...);
		_script->_entity = _owner;
	}

	/**
	 * Detach a script from the script component
	 */
	void Detach(){
		_script.reset();
	}

	// Base script abstraction methods
	void OnInit() const
	{
		if (_script){
			_script->OnInit();
		}
	}

	void OnUpdate(float deltaTime) const
	{
		if (_script){
			_script->OnUpdate(deltaTime);
		}
	}

	void OnClose() const
	{
		if (_script){
			_script->OnClose();
		}
	}

	void OnCollisionEnter(Entity* other, Collision* collision) const
	{
		if (_script){
			_script->OnCollisionEnter(other, collision);
		}
	}

	void OnCollisionExit(Entity* other, Collision* collision) const
	{
		if (_script){
			_script->OnCollisionExit(other, collision);
		}
	}
};

#endif