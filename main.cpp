#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600} ), "Carbon Footprint Calculator");

    tgui::Gui gui{window};

    auto root = tgui::Panel::create({"100%", "100%"});
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
    return 0;
}
