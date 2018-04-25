#include <overkill/Scene.hpp>


namespace overkill
{
    // EntityCamera* Scene::m_camera;
    std::vector<Entity*> Scene::m_entities;
    std::vector<int> Scene::m_rootEntities;

    Scene::Scene()
    {

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
    }


    void Scene::loadScene()
    {   
        int count = 0;
        auto camera = new EntityCamera("camera", count++, glm::vec3(0, 0, -40), glm::vec3(30, 0, 0), glm::vec3(3, -1, 0), glm::vec3(-5, 6, 0));

        auto modelCubeObject = new EntityModel("cube", "parentCube", count++, glm::vec3(0, 0, 0), 
                                glm::vec3(0), glm::vec3(1),
                                glm::vec3(0, 0, 0), 
                                glm::vec3(1, 3.4f, 1.67f));
        auto modelCubeChildObject = new EntityModel("cube", "childCube", count++, glm::vec3(2.0f, 0, 0));

        auto modelSuzanneObject = new EntityModel("Suzanne", "Suzanne", count++, glm::vec3(4,10,1),
                                glm::vec3(45, 45, 45),glm::vec3(5, 5, 5),
                                glm::vec3(0), glm::vec3(1, 3.4f, 1.67f));

        auto modelFloorObject = new EntityModel("cube", "floor", count++, glm::vec3(0, -3, 0), glm::vec3(0), glm::vec3(20, 0.5f, 20));
        
        addEntity(camera);    // Add models to container. Any changes made after this will be lost.
        addEntity(modelCubeObject);    // Add models to container. Any changes made after this will be lost.
        addEntity(modelCubeChildObject);
        addEntity(modelSuzanneObject);    // Add models to container. Any changes made after this will be lost.
        addEntity(modelFloorObject);    // Add models to container. Any changes made after this will be lost.

        setChild(modelCubeObject-> getEntityID(), modelCubeChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.
    }

    int Scene::addEntity(Entity* entity)
    {
        int ID = m_entities.size();
        m_rootEntities.push_back(ID);
        m_entities.push_back(entity);

        return ID;
    }

    Entity* Scene::getEntity(int ID)
    {
        return m_entities[ID];
    }

    // Entity* Scene::getEntityByTag(const C::Tag tag)
    // {
    //     auto comp = [tag](Entity* entity)       // Lambda to compare two entities by tag.
    //     {
    //         return (entity-> getTag() == tag);
    //     };

    //     auto entityToGet = std::find(m_entities.begin(), m_entities.end(), comp);
    //     // if (entityToGet == m_entities.end())
    //     // {
    //     //     LOG_ERROR("Attempt to find entity that does not exist.");
    //     // }
    //     return *entityToGet;
    // }

    void Scene::update(float dt)
    {
        for (auto ID : m_rootEntities)
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