#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

#include "Core/Profiles.h"
#include "Core/Calculator.h"


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

    // create calculator
    calculator mainCalculator = calculator();

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

                mainCalculator.calculate(userProfile->getParameters());

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
                mainCalculator.calculate(userProfile->getParameters());
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

            bool valuesChanged = false;
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

                    if (userProfile->update_carbon_source(mapKey, mapValue)) valuesChanged = true;
                }
            }

            // recalculate data if changed



            mainWindow.clear();
            //renders window
            mainGui.draw();
 
            // calculate values
            if (valuesChanged)
            {
                mainCalculator.calculate(userProfile->getParameters());
            }
            std::unordered_map<std::string, float> tempValues = mainCalculator.get_values();

            float travelSum = tempValues["GasUsageInput"] + tempValues["WalkingInput"] + tempValues["BusInput"] + tempValues["BikeInput"] + tempValues["FlyingInput"];
            float homeSum = tempValues["HousePowerInput"] + tempValues["NaturalGasInput"];
            float waterSum = tempValues["BottledWaterInput"] + tempValues["TapWaterInput"] + tempValues["ShowerCountInput"];
            float foodSum = tempValues["BeefInput"] + tempValues["PorkInput"] + tempValues["ChickenInput"] + tempValues["MuttonInput"] + tempValues["BreadInput"] + tempValues["RiceInput"] + tempValues["FruitInput"] + tempValues["VegetableInput"];

            int currOffset = 0;
            int barOffset = 80;
            int roof = 640;
            int floor = 980;
            int individualWidth = 60;
            std::vector<float> sumVector = { travelSum, homeSum, waterSum, foodSum };

            float largestSum = 0.0f;
            for (float currSum : sumVector)
            {
                if (currSum > largestSum)
                {
                    largestSum = currSum;
                }
            }
            
            mainRoot->get<tgui::Panel>("GraphPanel")->get<tgui::Label>("Unit Label 1")->setText(std::to_string(static_cast<int>(largestSum)));
            mainRoot->get<tgui::Panel>("GraphPanel")->get<tgui::Label>("Unit Label 2")->setText(std::to_string(static_cast<int>(largestSum/4*3)));
            mainRoot->get<tgui::Panel>("GraphPanel")->get<tgui::Label>("Unit Label 3")->setText(std::to_string(static_cast<int>(largestSum/4*2)));
            mainRoot->get<tgui::Panel>("GraphPanel")->get<tgui::Label>("Unit Label 4")->setText(std::to_string(static_cast<int>(largestSum/4*1)));


            int currBarIndex = 0;
            sf::RectangleShape bars[4];
            for (auto currBar : bars)
            {
                currOffset = currOffset + barOffset;
                float barHeight = sumVector[currBarIndex] / largestSum * 340;
                currBar.setPosition(tgui::Vector2f(static_cast<float>(currOffset), static_cast<float>(roof + 340 - barHeight)));
                currBar.setSize(tgui::Vector2f(static_cast<float>(individualWidth), static_cast<float>(barHeight)));
                currBarIndex++;
                currBar.setFillColor(sf::Color::Blue);
                mainWindow.draw(currBar);
            }


            std::unordered_map<std::string, float> tempPercents = mainCalculator.get_percentages();

            float travelPercentSum = tempPercents["GasUsageInput"] + tempPercents["WalkingInput"] + tempPercents["BusInput"] + tempPercents["BikeInput"] + tempPercents["FlyingInput"];
            float homePercentSum = tempPercents["HousePowerInput"] + tempPercents["NaturalGasInput"];
            float waterPercentSum = tempPercents["BottledWaterInput"] + tempPercents["TapWaterInput"] + tempPercents["ShowerCountInput"];
            float foodPercentSum = tempPercents["BeefInput"] + tempPercents["PorkInput"] + tempPercents["ChickenInput"] + tempPercents["MuttonInput"] + tempPercents["BreadInput"] + tempPercents["RiceInput"] + tempPercents["FruitInput"] + tempPercents["VegetableInput"];

            currOffset = 400;
            barOffset = 80;
            roof = 640;
            floor = 980;
            individualWidth = 60;
            std::vector<float> percentSumVector = { travelPercentSum, homePercentSum, waterPercentSum, foodPercentSum };



            currBarIndex = 0;
            sf::RectangleShape percentBars[4];
            for (auto currBar : percentBars)
            {
                currOffset = currOffset + barOffset;
                float barHeight = percentSumVector[currBarIndex] * 340;
                currBar.setPosition(tgui::Vector2f(static_cast<float>(currOffset), static_cast<float>(roof + 340 - barHeight)));
                currBar.setSize(tgui::Vector2f(static_cast<float>(individualWidth), static_cast<float>(barHeight)));
                currBarIndex++;
                currBar.setFillColor(sf::Color::Blue);
                mainWindow.draw(currBar);
            }

            mainWindow.display();
        }
    }

    return 0;
}