#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include <iostream>

#include "Core/Profiles.h"

int main()
{
    /* User profiles test.
    Create a profiles instance, add a profile, then delete the instance.
    Then create another instance and attempt to log in to the previously created profile.
    The created profile should persist as it was saved to disc.*/
    profile_manager* user_profiles = new profile_manager();
    user_profiles->create_profile("Lewis", "mypassword");
    profile* my_profile = user_profiles->login("Lewis", "mypassword");
    my_profile->add_carbon_source("Source", 15.5f);
    my_profile->add_carbon_source("Other Source", 37.1f);
    delete user_profiles;


    user_profiles = new profile_manager();
    profile* profile = user_profiles->login("Lewis", "mypassword");
    if (profile != nullptr) {
        std::cout << profile->get_carbon_from_source("Source") << std::endl;
        std::cout << profile->get_carbon_from_source("Other Source") << std::endl;
    }
    else {
        std::cout << "NO" << std::endl;
    }
    delete user_profiles;

    /*sf::RenderWindow window(sf::VideoMode({800, 600}), "Carbon Footprint Calculator");

    tgui::Gui gui{ window };

    auto root = tgui::Panel::create({ "100%", "100%" });
    gui.add(root);

    auto title = tgui::Label::create("Carbon Footprint Calculator ");
    title->setPosition(250, 20);
    title->setTextSize(22);
    root->add(title);

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            gui.handleEvent(*event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        gui.draw();
        window.display();
    }

    return 0;*/
}
