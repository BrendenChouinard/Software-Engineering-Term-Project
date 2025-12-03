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
    /*profile_manager* user_profiles = new profile_manager();
    user_profiles->create_profile("Lewis", "mypassword");
    profile* my_profile = user_profiles->login("User", "mypassword");
    my_profile->add_carbon_source("Source", 15.5f);
    my_profile->add_carbon_source("Other Source", 37.1f);
    delete user_profiles;


    user_profiles = new profile_manager();
    profile* profile = user_profiles->login("User", "mypassword");
    if (profile != nullptr) {
        std::cout << profile->get_carbon_from_source("Source") << std::endl;
        std::cout << profile->get_carbon_from_source("Other Source") << std::endl;
    }
    else {
        std::cerr << "Invalid Login" << std::endl;
    }
    delete user_profiles;*/

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Carbon Footprint Calculator");

    tgui::Gui gui{ window };

    auto root = tgui::Panel::create({ "100%", "100%" });
    gui.add(root);

    auto title = tgui::Label::create("Carbon Footprint Calculator ");
    title->setPosition(250, 20);
    title->setTextSize(22);
    root->add(title);

    //creates a panel widget where login widgets go
    auto form = tgui::Panel::create({500, 260});
    //half of the width - panel width, half of the height - panel height (centered)
    form->setPosition("50% - 250", "50% - 130");
    root->add(form);

    //makes login header positioned to the side
    auto heading = tgui::Label::create("Login");
    heading->setTextSize(20);
    heading->setPosition(20,15);
    form->add(heading);

    //Username subheader below login
    auto userLabel = tgui::Label::create("Username");
    userLabel->setPosition(20, 60);
    form->add(userLabel);

    //creates box where user enters username (input)
    auto userEntry = tgui::EditBox::create();
    userEntry->setSize(300, 32);
    userEntry->setPosition(150, 55);
    userEntry->setDefaultText("enter username");
    form->add(userEntry);

    //password header below username header
    auto passwordLabel = tgui::Label::create("Password");
    passwordLabel->setPosition(20, 110);
    form->add(passwordLabel);

    //creates password box where user enters password(input)
    auto passwordEntry = tgui::EditBox::create();
    passwordEntry->setSize(300, 32);
    passwordEntry->setPosition(150, 105);
    passwordEntry->setDefaultText("enter password");
    passwordEntry->setPasswordCharacter(U'*');
    form->add(passwordEntry);

    //login button
    auto loginButton = tgui::Button::create("Login");
    loginButton->setSize(120, 36);
    loginButton->setPosition(20, 170);
    form->add(loginButton);

    //create account button
    auto createButton = tgui::Button::create("Create Account");
    createButton->setSize(160, 36);
    createButton->setPosition(160, 170);
    form->add(createButton);

    //forgot password button
    auto forgotButton = tgui::Button::create("Forgot Password");
    forgotButton->setSize(160, 36);
    forgotButton->setPosition(330, 170);
    form->add(forgotButton);



    //presses buttons that we have created
    loginButton->onPress([]{ std::cout << "Login not yet working."; });
    createButton->onPress([&]{ std::cout << "create account not yet working."; });
    forgotButton->onPress([&]{ std::cout << "password recovery not yet working"; });

    //runs while the window is open
    //gives each event to TGUI to handle
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            gui.handleEvent(*event);
            if (event->is<sf::Event::Closed>())
                //closes window, exits loop
                window.close();
        }

        window.clear();
        //renders window
        gui.draw();
        window.display();
    }

    return 0;
}
