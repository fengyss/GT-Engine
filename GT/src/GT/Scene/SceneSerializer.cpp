#include "gtpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include <yaml-cpp/yaml.h>
#include "GT/Project/Project.h"
#include "GT/Assets/AssetsManager.h"
#include "GT/Particle/ParticleSystem.h"


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
	//template<>
	//struct convert<GT::EmitterShape>
	//{
	//	static Node encode(const GT::EmitterShape& rhs)
	//	{
	//		Node node;
	//		node = (int)rhs;
	//		node.SetStyle(EmitterStyle::Flow);
	//		return node;
	//	}

	//	static bool decode(const Node& node, GT::EmitterShape& rhs)
	//	{
	//		if (!node.IsScalar())
	//			return false;

	//		rhs = static_cast<GT::EmitterShape>(node[0].as<int>());
	//		return true;
	//	}
	//};
	//template<>
	//struct convert<GT::UUID>
	//{
	//	static Node encode(const GT::UUID& uuid)
	//	{
	//		Node node;
	//		node.push_back((uint64_t)uuid);
	//		return node;
	//	}

	//	static bool decode(const Node& node, GT::UUID& uuid)
	//	{
	//		uuid = node.as<uint64_t>();
	//		return true;
	//	}
	//};

}


namespace GT
{

#define WRITE_SCRIPT_FIELD(FieldType, Type)           \
			case ScriptFieldType::FieldType:          \
				out << scriptField.GetValue<Type>();  \
				break

#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Data"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		GT_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		GT_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}


	static void SerializeEntity(YAML::Emitter& out, Entity entity);
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{
	}



	void SceneSerializer::Serialize(std::filesystem::path& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto view = m_Scene->m_Registry.view<TagComponent>();
		for (auto e : view)
		{
			Entity entity{ e,m_Scene.get() };
			if (!entity)
				return;
			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	bool SceneSerializer::Deserialize(std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			GT_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filepath.string(), e.what());
			return false;
		}


		if (!data["Scene"])
			return false;


		std::string sceneName = filepath.stem().string();
		GT_CORE_INFO("Deserializing scene '{0}'", sceneName);

		m_Scene->SetFilePath(filepath);
		m_Scene->SetName(sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				std::string type;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
					type = tagComponent["Type"].as<std::string>();
				}

				GT_CORE_INFO("Deserialized [{2}] entity with ID = {0}, name = {1}", uuid, name,type);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);
				deserializedEntity.GetComponent<TagComponent>().Type = type;

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFov(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Offset = cameraProps["Offset"].as<glm::vec3>();
					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}


				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["TexturePath"])
					{
						std::filesystem::path texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						//auto path = Project::GetAssetFileSystemPath(texturePath);
						if(!std::filesystem::exists(texturePath))
							GT_CORE_ERROR("Texture file '{0}' does not exist", texturePath.string());
						else src.texture = CreateHandle<Texture2D>(texturePath);
					}

					if (spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto modelComponent = entity["ModelComponent"];
				if (modelComponent)
				{
					auto& src = deserializedEntity.AddComponent<ModelComponent>();
						std::string modelPath = modelComponent["ModelPath"].as<std::string>();

						if (!std::filesystem::exists(modelPath))
							GT_CORE_ERROR("Model file '{0}' does not exist", modelPath);
						else src.model = CreateHandle<Model>(modelPath);
				}

				auto lightComponent = entity["LightComponent"];
				if (lightComponent)
				{
					auto& src = deserializedEntity.AddComponent<LightRendererComponent>();
					if (lightComponent["TexturePath"])
					{
						std::filesystem::path texturePath = lightComponent["TexturePath"].as<std::string>();
						//auto path = Project::GetAssetFileSystemPath(texturePath);
						if (!std::filesystem::exists(texturePath))
							GT_CORE_ERROR("Texture file '{0}' does not exist", texturePath.string());
						else src.texture = CreateHandle<Texture2D>(texturePath);
					}
					auto& light = src.light;
					light.type = (LightType)lightComponent["Type"].as<int>();
					light.ambient= lightComponent["Ambient"].as<glm::vec3>();
					light.constant= lightComponent["Constant"].as<float>();
					light.cutOff= lightComponent["cutOff"].as<float>();
					light.outerCutOff= lightComponent["outerCutOff"].as<float>();
					light.diffuse= lightComponent["Diffuse"].as<glm::vec3>();
					light.direction= lightComponent["Direction"].as<glm::vec3>();
					light.linear= lightComponent["Linear"].as<float>();
					light.pos = lightComponent["Position"].as<glm::vec3>();
					light.quadratic= lightComponent["Quadratic"].as<float>();
					light.specular = lightComponent["Specular"].as<glm::vec3>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();

					if (circleRendererComponent["TexturePath"])
					{
						std::filesystem::path texturePath = circleRendererComponent["TexturePath"].as<std::string>();
						//auto path = Project::GetAssetFileSystemPath(texturePath);
						if (!std::filesystem::exists(texturePath))
							GT_CORE_ERROR("Texture file '{0}' does not exist", texturePath.string());
						else crc.texture = CreateHandle<Texture2D>(texturePath);
					}
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.ClassName = scriptComponent["ClassName"].as<std::string>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc2d.Density = circleCollider2DComponent["Density"].as<float>();
					cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}
				
				auto animator2DComponent = entity["Animator2DComponent"];
				if (animator2DComponent)
				{
					auto& anim = deserializedEntity.AddComponent<Animator2DComponent>();
				}

				auto particleComponent = entity["ParticleComponent"];
				if (particleComponent)
				{
					auto& pc = deserializedEntity.AddComponent<ParticleComponent>();
					pc.IsEmitting = particleComponent["IsEmitting"].as<bool>();

					auto& config = pc.Config;
					config.shape = (EmitterShape)particleComponent["EmitterShape"].as<int>();
					config.blendMode = (BlendMode)particleComponent["BlendMode"].as<int>();
					config.direction = particleComponent["Direction"].as<glm::vec3>();
					config.colorVariance = particleComponent["Color"].as<glm::vec4>();
					config.velocity = particleComponent["Velocity"].as<float>();
					config.radius = particleComponent["Radius"].as<float>();
					config.InUnitSphere = particleComponent["InUnitSphere"].as<bool>();

					config.spawnRate = particleComponent["SpawnRate"].as<float>();
					config.lifetime = particleComponent["LifeTime"].as<float>();
					config.innerRadius = particleComponent["InnerRadius"].as<float>();
					config.outerRadius = particleComponent["OuterRadius"].as<float>();
					config.coneAngle = particleComponent["ConeAngle"].as<float>();


					YAML::Node burstsNode = particleComponent["Bursts"];
					
					for (auto& burst : burstsNode)
					{
						ParticleBurst b;
						b.time = burst["Time"].as<float>();
						b.count = burst["Count"].as<uint32_t>();
						b.cycles = burst["Cycles"].as<uint32_t>();
						b.interval = burst["Interval"].as<float>();
						config.bursts.push_back(b);
					}
					pc.IsRegen = true;
				}


				//auto textComponent = entity["TextComponent"];
				//if (textComponent)
				//{
				//	auto& tc = deserializedEntity.AddComponent<TextComponent>();
				//	tc.TextString = textComponent["TextString"].as<std::string>();
				//	// tc.FontAsset // TODO
				//	tc.Color = textComponent["Color"].as<glm::vec4>();
				//	tc.Kerning = textComponent["Kerning"].as<float>();
				//	tc.LineSpacing = textComponent["LineSpacing"].as<float>();
				//}
				GT_CORE_INFO("Deserialized [{1}] entity {0} End!", name, type);
			}
		}
		return true;
	}
	bool SceneSerializer::DeserializeRuntime(std::filesystem::path& filepath)
	{
		//not implemented
		GT_CORE_ASSERT(false, "Not implemented");
		return false;
	}

	void SceneSerializer::SerializeRuntime(std::filesystem::path& filepath)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		GT_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity didn't have UUID!");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Tag" << YAML::Value << tag.Tag;
			out << YAML::Key << "Type" << YAML::Value << tag.Type;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // ScriptComponent
			out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;
			out << YAML::EndMap; // ScriptComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFov();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::Key << "Offset" << YAML::Value << cameraComponent.Offset;
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		//if (entity.HasComponent<ScriptComponent>())
		//{
		//	auto& scriptComponent = entity.GetComponent<ScriptComponent>();

		//	out << YAML::Key << "ScriptComponent";
		//	out << YAML::BeginMap; // ScriptComponent
		//	out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

		//	// Fields
		//	Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
		//	const auto& fields = entityClass->GetFields();
		//	if (fields.size() > 0)
		//	{
		//		out << YAML::Key << "ScriptFields" << YAML::Value;
		//		auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
		//		out << YAML::BeginSeq;
		//		for (const auto& [name, field] : fields)
		//		{
		//			if (entityFields.find(name) == entityFields.end())
		//				continue;

		//			out << YAML::BeginMap; // ScriptField
		//			out << YAML::Key << "Name" << YAML::Value << name;
		//			out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

		//			out << YAML::Key << "Data" << YAML::Value;
		//			ScriptFieldInstance& scriptField = entityFields.at(name);

		//			switch (field.Type)
		//			{
		//				WRITE_SCRIPT_FIELD(Float, float);
		//				WRITE_SCRIPT_FIELD(Double, double);
		//				WRITE_SCRIPT_FIELD(Bool, bool);
		//				WRITE_SCRIPT_FIELD(Char, char);
		//				WRITE_SCRIPT_FIELD(Byte, int8_t);
		//				WRITE_SCRIPT_FIELD(Short, int16_t);
		//				WRITE_SCRIPT_FIELD(Int, int32_t);
		//				WRITE_SCRIPT_FIELD(Long, int64_t);
		//				WRITE_SCRIPT_FIELD(UByte, uint8_t);
		//				WRITE_SCRIPT_FIELD(UShort, uint16_t);
		//				WRITE_SCRIPT_FIELD(UInt, uint32_t);
		//				WRITE_SCRIPT_FIELD(ULong, uint64_t);
		//				WRITE_SCRIPT_FIELD(Vector2, glm::vec2);
		//				WRITE_SCRIPT_FIELD(Vector3, glm::vec3);
		//				WRITE_SCRIPT_FIELD(Vector4, glm::vec4);
		//				WRITE_SCRIPT_FIELD(Entity, UUID);
		//			}
		//			out << YAML::EndMap; // ScriptFields
		//		}
		//		out << YAML::EndSeq;
		//	}

		//	out << YAML::EndMap; // ScriptComponent
		//}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			if (spriteRendererComponent.texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.texture->GetPath().string();

			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<ModelComponent>())
		{
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap; 

			auto& modelComponent = entity.GetComponent<ModelComponent>();
				out << YAML::Key << "ModelPath" << YAML::Value << modelComponent.model->Get()->filepath.string();

			out << YAML::EndMap; 
		}

		if (entity.HasComponent<LightRendererComponent>())
		{
			out << YAML::Key << "LightComponent";
			out << YAML::BeginMap; // CubeComponent

			auto& lightComponent = entity.GetComponent<LightRendererComponent>();
			if (lightComponent.texture)
				out << YAML::Key << "TexturePath" << YAML::Value << lightComponent.texture->GetPath().string();

			auto& light = lightComponent.light;
			out << YAML::Key << "Type" << YAML::Value << (int)light.type;
			out << YAML::Key << "Ambient" << YAML::Value << light.ambient;
			out << YAML::Key << "Constant" << YAML::Value << light.constant;
			out << YAML::Key << "cutOff" << YAML::Value << light.cutOff;
			out << YAML::Key << "outerCutOff" << YAML::Value << light.outerCutOff;
			out << YAML::Key << "Diffuse" << YAML::Value << light.diffuse;
			out << YAML::Key << "Direction" << YAML::Value << light.direction;
			out << YAML::Key << "Linear" << YAML::Value << light.linear;
			out << YAML::Key << "Position" << YAML::Value << light.pos;
			out << YAML::Key << "Quadratic" << YAML::Value << light.quadratic;
			out << YAML::Key << "Specular" << YAML::Value << light.specular;


			out << YAML::EndMap; // CubeComponent
		}
		
		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent

			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			if (circleRendererComponent.texture)
				out << YAML::Key << "TexturePath" << YAML::Value << circleRendererComponent.texture->GetPath().string();

			out << YAML::EndMap; // CircleRendererComponent
		}
		
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent

			auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

			out << YAML::EndMap; // Rigidbody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent

			auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

			out << YAML::EndMap; // BoxCollider2DComponent
		}

		
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent

			auto& cc2dComponent = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

			out << YAML::EndMap; // CircleCollider2DComponent
		}
		if (entity.HasComponent<Animator2DComponent>())
		{
			out << YAML::Key << "Animator2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent

			auto& animComponent = entity.GetComponent<Animator2DComponent>();

			out << YAML::EndMap; // CircleCollider2DComponent
		}
		if (entity.HasComponent<ParticleComponent>())
		{
			out << YAML::Key << "ParticleComponent";
			out << YAML::BeginMap;

			auto& pc = entity.GetComponent<ParticleComponent>();
			auto& config = pc.Config;
			out << YAML::Key << "IsEmitting" << YAML::Value << pc.IsEmitting;
			out << YAML::Key << "EmitterShape" << YAML::Value << (int)config.shape;
			out << YAML::Key << "BlendMode" << YAML::Value << (int)config.blendMode;
			out << YAML::Key << "Direction" << YAML::Value << config.direction;
			out << YAML::Key << "Color" << YAML::Value << config.colorVariance;
			out << YAML::Key << "Velocity" << YAML::Value << config.velocity;
			out << YAML::Key << "Radius" << YAML::Value << config.radius;
			out << YAML::Key << "InUnitSphere" << YAML::Value << config.InUnitSphere;

			out << YAML::Key << "SpawnRate" << YAML::Value << config.spawnRate;
			out << YAML::Key << "LifeTime" << YAML::Value << config.lifetime;
			out << YAML::Key << "InnerRadius" << YAML::Value << config.innerRadius;
			out << YAML::Key << "OuterRadius" << YAML::Value << config.outerRadius;
			out << YAML::Key << "ConeAngle" << YAML::Value << config.coneAngle;

			out << YAML::Key << "Bursts";

			out << YAML::BeginSeq;
			for (const auto& burst : config.bursts)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Time" << YAML::Value << burst.time;
				out << YAML::Key << "Count" << YAML::Value << burst.count;
				out << YAML::Key << "Cycles" << YAML::Value << burst.cycles;
				out << YAML::Key << "Interval" << YAML::Value << burst.interval;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;


			out << YAML::EndMap;

		}
		/*
		if (entity.HasComponent<TextComponent>())
		{
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap; // TextComponent

			auto& textComponent = entity.GetComponent<TextComponent>();
			out << YAML::Key << "TextString" << YAML::Value << textComponent.TextString;
			// TODO: textComponent.FontAsset
			out << YAML::Key << "Color" << YAML::Value << textComponent.Color;
			out << YAML::Key << "Kerning" << YAML::Value << textComponent.Kerning;
			out << YAML::Key << "LineSpacing" << YAML::Value << textComponent.LineSpacing;

			out << YAML::EndMap; // TextComponent
		}
		*/


		out << YAML::EndMap; // Entity
	}

}