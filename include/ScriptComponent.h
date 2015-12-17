#ifndef SCRIPT_COMPONENT_H
#define SCRIPT_COMPONENT_H

// ECS
#include "Component.h"

// Scripting
#include "IScript.h"

// STD
#include <vector>
#include <type_traits>
#include <utility>

class ScriptComponent : public Component<ScriptComponent>
{
private:
	std::vector<IScript*> scripts;

public:
	virtual ~ScriptComponent(void)
    {
		for (auto& script : scripts)
        {
            if(script)
            {
                delete script;
            }
		}
	}

	void Update(GameEntity& entity, float dt, float tt)
    {
        for(auto& script : scripts)
        {
            script->Update(entity, dt, tt);
        }
	}

    template <typename T, typename... Args>
    void AddScript(Args&&... args)
    {
        static_assert(std::is_base_of<IScript, T>::value, "Must be an instance of IScript!");

        IScript* pScript = new T(std::forward<Args>(args)...);
        scripts.push_back(pScript);
	}

	void RemoveScript(IScript* script)
    {
        // C++ Erase-Remove Idiom
        scripts.erase
        (
            std::remove
            (
                scripts.begin(),
                scripts.end(),
                script
            ),
            scripts.end()
        );

	}
};

#endif
