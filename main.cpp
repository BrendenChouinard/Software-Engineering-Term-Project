#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include <iostream>
#include <vector>

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

    sf::RenderWindow mainWindow(sf::VideoMode({800, 1000}), "Carbon Footprint Calculator");

    tgui::Gui mainGui{ mainWindow };

    auto mainRoot = tgui::Panel::create({ "100%", "100%" });
    mainGui.add(mainRoot);

    mainRoot->loadWidgetsFromFile("../../../main.txt");



    sf::RenderWindow loginWindow(sf::VideoMode({ 800, 800 }), "Carbon Footprint Calculator");

    tgui::Gui loginGui{ loginWindow };

    auto loginRoot = tgui::Panel::create({ "100%", "100%" });
    loginGui.add(loginRoot);

    bool loggedIn = false;


    auto title = tgui::Label::create("Carbon Footprint Calculator ");
    title->setPosition(250, 20);
    title->setTextSize(22);
    loginRoot->add(title);

    //creates a panel widget where login widgets go
    auto form = tgui::Panel::create({500, 260});
    //half of the width - panel width, half of the height - panel height (centered)
    form->setPosition("50% - 250", "50% - 130");
    loginRoot->add(form);

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
    loginButton->setPosition(60, 170);
    form->add(loginButton);

    //create account button
    auto createButton = tgui::Button::create("Create Account");
    createButton->setSize(160, 36);
    createButton->setPosition(250, 170);
    form->add(createButton);

    // load profiles
    profile_manager currProfiles = profile_manager();
    
    // set default profile
    profile* userProfile = nullptr;

    //presses buttons that we have created
    loginButton->onPress([&]
        {
            // get username/password in std strings
            tgui::String tempUsername = userEntry->getText();
            std::string username = tempUsername.toStdString();
            tgui::String tempPassword = passwordEntry->getText();
            std::string password = tempPassword.toStdString();

            // attempt login
            userProfile = currProfiles.login(username, password);
            
            if (userProfile != nullptr) // success
            {
                // load data into main edit boxes
                // for each widget
                for (auto& widget : mainRoot->get<tgui::ScrollablePanel>("MainDataEntryPanel")->getWidgets())
                {
                    // if it is an editbox
                    if (auto edit = widget->cast<tgui::EditBox>())
                    {
                        // get update value at box
                        tgui::String boxName = edit->getWidgetName();
                        std::string mapKey = boxName.toStdString();
                        edit->setText(userProfile->get_carbon_from_source(mapKey));
                    }
                }

                // set logged in, and close login window
                loggedIn = true;
                loginWindow.close();
            }
            else
            {
                auto failedMessage = tgui::MessageBox::create("Login failed", "Incorrect username or password", { "OK" });
                loginGui.add(failedMessage);
                // center
                failedMessage->setPosition(300, 317);

                // make it close
                failedMessage->onButtonPress([failedMessage](const tgui::String& button){
                    failedMessage->close();
                });
            }
        });
    createButton->onPress([&]
        {
            // get username/password in std strings
            tgui::String tempUsername = userEntry->getText();
            std::string username = tempUsername.toStdString();
            tgui::String tempPassword = passwordEntry->getText();
            std::string password = tempPassword.toStdString();

            // attempt profile creation
            bool profileCreationSuccess = currProfiles.create_profile(username, password);
            if (profileCreationSuccess) // success
            {
                userProfile = currProfiles.login(username, password);
                loggedIn = true;
                loginWindow.close();
            }
            else
            {
                auto failedMessage = tgui::MessageBox::create("Profile creation failed", "Profiles with this username already exists", { "OK" });
                loginGui.add(failedMessage);
                // center
                failedMessage->setPosition(300, 317);

                // make it close
                failedMessage->onButtonPress([failedMessage](const tgui::String& button) {
                    failedMessage->close();
                    });
            }
        });


    //runs while the window is open
    //gives each event to TGUI to handle
    while (mainWindow.isOpen())
    {
        
        if (!loggedIn)
        {
            while (const std::optional<sf::Event> event = loginWindow.pollEvent())
            {
                loginGui.handleEvent(*event);
                if (event->is<sf::Event::Closed>())
                    //closes window, exits loop
                    mainWindow.close();
            }

            loginWindow.clear();
            //renders window
            loginGui.draw();
            loginWindow.display();
        }
        else
        {
            while (const std::optional<sf::Event> event = mainWindow.pollEvent())
            {
                mainGui.handleEvent(*event);
                if (event->is<sf::Event::Closed>())
                    //closes window, exits loop
                    mainWindow.close();
            }

            auto panel = mainRoot->get<tgui::ScrollablePanel>("MainDataEntryPanel");

            if (!panel)
                std::cout << "Panel not found\n";

            // for each widget
            for (auto& widget : mainRoot->get<tgui::ScrollablePanel>("MainDataEntryPanel")->getWidgets())
            {
                // if it is an editbox
                if (auto edit = widget->cast<tgui::EditBox>())
                {
                    // get box name
                    tgui::String boxName = edit->getWidgetName();
                    std::string mapKey = boxName.toStdString();

                    // get data
                    tgui::String boxValue = edit->getText();
                    std::string mapValue = boxValue.toStdString();

                    userProfile->update_carbon_source(mapKey, mapValue);
                }
            }

            mainWindow.clear();
            //renders window
            mainGui.draw();
            mainWindow.display();
        }
    }

    return 0;
}




// Calculate Carbon from each source
// 
// Sum carbon from sections
// 
// make graph(s)