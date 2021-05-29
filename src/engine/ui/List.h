#pragma once

#include "elements/UIElement.h"

namespace UI
{
    // TODO: rewrite this class, wtf is that
    template<class T>
    class List :
        public UIElement
    {
    private:
        std::map<std::string, T*> _container;

    public:
        List(GameDataRef data)
            :UIElement(data)
        {
            static_assert(std::is_base_of<UIElement, T>::value,
                "Panel class can only cintain UI Elements (List.h, in constructor)");
        }
        ~List();

        // getters
        T& GetElement(std::string name) const;

        // setters
        T& AddElement(std::string name, T&& new_element);

        void update(float deltaTime) override;
        void render() const override;

    };

    template<class T>
    inline List<T>::~List()
    {
        // deleting all elements
        for (auto& [name, element] : this->_container)
            delete element;
    }

    template<class T>
    inline T& List<T>::GetElement(std::string name) const
    {
        return this->_container.at(name);
    }

    template<class T>
    inline T& List<T>::AddElement(std::string name, T&& new_element)
    {
        this->_container[name] = new T(std::forward<T>(new_element));
    }

    template<class T>
    inline void List<T>::update(float deltaTime)
    {
        // TODO: update elements and add some input handling
    }

    template<class T>
    inline void List<T>::render() const
    {
        // rendering all elements
        for (const auto& [name, element] : this->_container)
            element->render();
    }

}
