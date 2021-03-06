#include <overkill/Scene.hpp>


namespace overkill
{
    // EntityCamera* Scene::m_camera;
    std::vector<Entity*> Scene::m_entities;
    std::vector<int> Scene::m_rootEntities;
    EntityCamera* Scene::m_activeCamera;
    int Scene::m_cameraCount;
    std::string Scene::m_sceneLoaded;

    Scene::Scene()
    {

    }


    void Scene::load(std::string sceneFile)
    {          
        // Reset values:
        int count = 0;
        int modelCount = 0;
        int relationsCount = 0;
        m_activeCamera = nullptr;
        m_cameraCount = 0;
        m_sceneLoaded = sceneFile;

        auto filestring = Util::fileToString(sceneFile);
        Parser p(filestring);

        LOG_INFO("Loading \n%s", filestring.c_str());

        //
        //  EntityCameras:
        //
        // int   cameraCount.
        if (auto[key, cameras, err] = p.keyInteger("cameras"); err) 
        {
            LOG_ERROR("%s error reading cameraCount on key --> %s...", filestring.c_str(), key.data());
        } 
        else 
        {
            m_cameraCount = cameras;
            LOG_INFO("%s: %d",key.data(), m_cameraCount);        
        }

        // Load EntityCameras.
        for (; count < m_cameraCount; count++)     
        {   
            C::Tag tag;
            glm::vec3 pos;
            glm::vec3 rot;
            glm::vec3 vel;
            glm::vec3 angVel;
            CameraMode camMode = FREELOOK;
            float camFov = 0;
            float aspect = 0;
            float nearClip = 0;
            float farClip = 0;

            // char  entityTag.    
            if (auto[key, camera, err] = p.keyString("camera"); err) 
            {
                LOG_ERROR("%s error on cameraTag key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                tag = camera;
                LOG_INFO("%s: %s",key.data(), tag.data());             
            }

            // vec3 position.
            if (auto[key, position, err] = p.keyVec3("position"); err) 
            {
                LOG_ERROR("%s error on camera position key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                pos = position;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), pos.x, pos.y, pos.z);                    
            }

            // vec3 rotation.
            if (auto[key, rotation, err] = p.keyVec3("rotation"); err) 
            {
                LOG_ERROR("%s error on camera rotation key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                rot = rotation;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), rot.x, rot.y, rot.z);                    
            }

            // vec3 velocity.
            if (auto[key, velocity, err] = p.keyVec3("velocity"); err) 
            {
                LOG_ERROR("%s error on camera velocity key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                vel = velocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), vel.x, vel.y, vel.z);                    
            }

            // vec3 rotation.
            if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err) 
            {
                LOG_ERROR("%s error on camera angle velocity key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                angVel = angleVelocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), angVel.x, angVel.y, angVel.z);                    
            }

            // char  CameraMode.    
            if (auto[key, mode, err] = p.keyString("mode"); err) 
            {
                LOG_ERROR("%s error on camera mode key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                if (!strcmp("freelook", mode.data()))
                {
                    camMode = FREELOOK;
                }
                else if (!strcmp("orbital", mode.data()))
                {
                    camMode = ORBITAL;
                }
                else
                {
                    LOG_ERROR("%s error on key --> %s... expected freelook or orbital.", filestring.c_str(), key.data());
                }
                LOG_INFO("%s: %s",key.data(), mode.data());                    
            }

            // float fov.
            if (auto[key, fov, err] = p.keyFloat("fov"); err) 
            {
                LOG_ERROR("%s error on camera fov key --> %s...", filestring.c_str(), key.data());
            }
            else
            {
                camFov = fov;
                LOG_INFO("%s: %f",key.data(), camFov);                    
            }

            // float aspectRatio.
            if (auto[key, aspectRatio, err] = p.keyFloat("aspectRatio"); err) 
            {
                LOG_ERROR("%s error on camera aspect ratio key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                aspect = aspectRatio;
                LOG_INFO("%s: %f",key.data(), aspect);                    
            }

            // float nearClip.
            if (auto[key, _nearClip, err] = p.keyFloat("nearClip"); err) 
            {
                LOG_ERROR("%s error on camera nearclip key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                nearClip = _nearClip;
                LOG_INFO("%s: %f",key.data(), nearClip);
            }

            // float farClip.
            if (auto[key, _farClip, err] = p.keyFloat("farClip"); err) 
            {
                LOG_ERROR("%s error on camera farclip key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                farClip = _farClip;
                LOG_INFO("%s: %f",key.data(), farClip);
            }

            auto camEntity = new EntityCamera(tag, 
                                              count, 
                                              pos, 
                                              rot, 
                                              vel, 
                                              angVel, 
                                              camMode, 
                                              camFov, 
                                              aspect, 
                                              nearClip,
                                              farClip);
            Scene::addEntity(camEntity);   
        }

        if (m_cameraCount == 0)
        {
            LOG_ERROR("m_cameraCount == 0: A scene has to have at least one camera");
        } 

        Scene::m_activeCamera = (EntityCamera*) Scene::m_entities[0];     // First camera in file is default camera.

        //
        //  EntityModels:
        //
        if (auto[key, entites, err] = p.keyInteger("entities"); err) 
        {
            LOG_ERROR("%s error on entityCount key --> %s...", filestring.c_str(), key.data());
        } 
        else 
        {
            modelCount = entites;
            LOG_INFO("%s: %d",key.data(), modelCount);        
        }


        for (; count < m_cameraCount +  modelCount; count++)     
        {   
            C::Tag entityTag;
            C::Tag modelTag;
            glm::vec3 pos;
            glm::vec3 rot;
            glm::vec3 scl;      // Scale.
            glm::vec3 vel;
            glm::vec3 angVel;

            // char  entityTag.    
            if (auto[key, entity, err] = p.keyString("entity"); err) 
            {
                LOG_ERROR("%s error on entityTag key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                entityTag = entity;
                LOG_INFO("%s: %s",key.data(), entityTag.data());                    
            }

            // char  modelTag.    
            if (auto[key, model, err] = p.keyString("model"); err) 
            {
                LOG_ERROR("%s error on entityModelTag key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                modelTag = model;
                LOG_INFO("%s: %s",key.data(), modelTag.data());                    
            }

            // vec3 position.
            if (auto[key, position, err] = p.keyVec3("position"); err) 
            {
                LOG_ERROR("%s error on entity position key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                pos = position;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), pos.x, pos.y, pos.z);                    
            }

            // vec3 rotation.
            if (auto[key, rotation, err] = p.keyVec3("rotation"); err) 
            {
                LOG_ERROR("%s error on entity rotation key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                rot = rotation;
                LOG_INFO("%s: (%f, %f, %f)", key.data(), rot.x, rot.y, rot.z);                    
            }

            // vec3 scale.
            if (auto[key, scale, err] = p.keyVec3("scale"); err) 
            {
                LOG_ERROR("%s error on entity scale key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                scl = scale;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), scl.x, scl.y, scl.z);                    
            }

            // vec3 velocity.
            if (auto[key, velocity, err] = p.keyVec3("velocity"); err) 
            {
                LOG_ERROR("%s error on entity velocity key --> %s...", filestring.c_str(), key.data());
            } 
            else 
             {
                vel = velocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), vel.x, vel.y, vel.z);                    
            }

            // vec3 angularVelocity.
            if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err) 
            {
                LOG_ERROR("%s error on entity angle velocity key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                angVel = angleVelocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), angVel.x, angVel.y, angVel.z);                    
            }


            if (modelTag == "NULL" || modelTag == "null" || modelTag == "none" || modelTag == "_")
            {
                auto nodeEntity =  new EntityNode(entityTag, 
                                    count, 
                                    pos, 
                                    rot,
                                    scl,
                                    vel, 
                                    angVel);
                addEntity((Entity*) nodeEntity); 
            }
            else 
            {
                auto modelEntity = new EntityModel(modelTag, 
                                    entityTag, 
                                    count, 
                                    pos, 
                                    rot,
                                    scl,
                                    vel, 
                                    angVel);
                addEntity((Entity*) modelEntity); 
            }

        }


       // Scene::m_directionalLights.resize(C::MAX_LIGHTS);
       // Scene::m_pointsLights.resize(C::MAX_LIGHTS);

/*
        // for ()      // Load EntityLights. NOT IMPLEMENTED.
        // {

        // }
*/

        if (auto[key, _relationsCount, err] = p.keyInteger("relations"); err )
        {
            LOG_ERROR("%s error on relationsCount key --> %s...", filestring.c_str(), key.data());
        }
        else 
        {
            LOG_DEBUG("relations: %d", _relationsCount);
            relationsCount = _relationsCount;
        }

        for (int i = 0; i < relationsCount; ++i)      // Set child-parent realtionships.
        {
            int childCount = 0;
            std::string parentTag;
            if (auto[key, _childCount, err] = p.nextKeyInteger(); err)
            {
                LOG_ERROR("%s error on childCount key --> %s...", filestring.c_str(), key.data());
            }
            else 
            {
                childCount = _childCount;
                parentTag = key;
            }

            for (int j = 0; j < childCount; ++j)
            {
                auto childTag = p.nextLine();
                Scene::setChild(
                    Scene::getEntityByTag(parentTag)->getEntityID(),
                    Scene::getEntityByTag(childTag)->getEntityID());
            }
        }

/*
            setChild(modelCubeObject -> getEntityID(), cameraTwo -> getEntityID());
            setChild(modelCubeChildObject -> getEntityID(), cameraThree -> getEntityID());
            setChild(modelCubeGrandChildObject -> getEntityID(), cameraFour -> getEntityID());

            setChild(camera->getEntityID(), debugCamPos->getEntityID());
            
            setChild(modelCubeObject-> getEntityID(), modelCubeChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.
            setChild(modelCubeChildObject-> getEntityID(), modelCubeGrandChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.
        
        
            m_activeCamera = (EntityCamera*) m_entities[0];
*/
                 // All cameras in the beginning of scene file, first one active by default.
        //
    }



    void Scene::reload() 
    {
        Init::Config();
        Init::OpenGL(C::ClearColor);
        glfwSetWindowSize(C::window, C::WinWidth, C::WinHeight);
        Scene::clean();
        Scene::m_entities.clear();
        Scene::m_rootEntities.clear();
        Scene::m_cameraCount = 0;
        Scene::m_activeCamera = nullptr;
        Scene::load(m_sceneLoaded);        
    }


    void Scene::setChild(int parentID, int childID)
    {
        auto rootToRemove = std::find(m_rootEntities.begin(), m_rootEntities.end(), childID);
        if (rootToRemove == m_rootEntities.end())
        {
            LOG_ERROR("Attempt to remove a root entity that does not exist(entiryID %d).", childID);
        }
        m_rootEntities.erase(rootToRemove);

        getEntity(parentID)-> addChild(childID);
    
        LOG_DEBUG("parent: %s --> child: %s",
            getEntity(parentID)-> getTag().data(),
            getEntity(childID)-> getTag().data());
    }


    bool Scene::entityExist(const C::Tag tag)
    {   
        bool exist = true;

        auto comp = [tag](Entity* entity)       // Lambda to compare two entities by tag.
        {
            return (entity-> getTag() == tag);
        };

        auto entityToGet = std::find_if(m_entities.begin(), m_entities.end(), comp);
        if (entityToGet == m_entities.end())
        {
            exist = false;
        }
        return exist;
    }



    int Scene::addEntity(Entity* entity)
    {
        int ID = -1;
        
        if (!entityExist(entity-> getTag()))  // If entity with this tag does not exist yet.                           
        {
            ID = m_entities.size();
            m_rootEntities.push_back(ID);
            m_entities.push_back(entity);
        }
        else                                                            // Entity with tag already exist.
        {
            LOG_ERROR("Attempt to create duplicate entity with tag: %s"
                    "\n\t\t\t\t  Check that your scene file does not contain two entities with the tag(%s).", 
                    entity-> getTag().c_str(), entity-> getTag().c_str());
        }

        return ID;
    }

    Entity* Scene::getEntity(int ID)
    {
        return m_entities[ID];
    }

    Entity* Scene::getEntityByTag(const C::Tag tag)
    {
        auto comp = [tag](Entity* entity)       // Lambda to compare two entities by tag.
        {
            return (entity-> getTag() == tag);
        };

        auto entityToGet = std::find_if(m_entities.begin(), m_entities.end(), comp);
        if (entityToGet == m_entities.end())
        {
            LOG_ERROR("Attempt to find entity that does not exist.");
        }
        return *entityToGet;
    }

    EntityCamera* Scene::getActiveCamera()
    {
        return m_activeCamera;
    }

    void Scene::cycleCameras()
    {
        m_activeCamera = (EntityCamera*) m_entities[(m_activeCamera->getEntityID() + 1) % m_cameraCount];
    }


    void Scene::update(float dt)
    {
        m_activeCamera -> checkInput();     // Only update the active camera on input.
        for (auto ID : m_rootEntities)      // all the cameras update() is still being run in here.
        {
            getEntity(ID)-> update(dt);
        }
    }

    void Scene::draw(float t)
    {
        for (Entity* entity : m_entities)
        {
            entity-> draw(t);
        }
    }

    void Scene::clean()
    {
        // delete m_camera;
        for (Entity* entity : m_entities)
        {
            delete entity;
        }
    }

}