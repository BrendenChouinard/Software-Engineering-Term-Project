#ifndef EXCLUSIVE_BUTTON_GROUP_H
#define EXCLUSIVE_BUTTON_GROUP_H

#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/RadioButton.hpp>
#include <vector>
#include <string>
#include <functional>

class ExclusiveButtonGroup {
public:
    struct Option { std::string id; std::string label; };
    using Callback = std::function<void(const std::string&)>;

    ExclusiveButtonGroup() { panel_ = tgui::Panel::create(); }

    void addOption(const std::string& id, const std::string& label) { options_.push_back({id,label}); }

    void build(float x, float y) {
        panel_->setPosition(x,y);
        float offsetY = 0.f;
        for (auto &opt : options_) {
            auto rb = tgui::RadioButton::create();
            rb->setText(opt.label);
            rb->setPosition(0, offsetY);
            rb->onCheck([this,opt]{ selected_ = opt.id; if (cb_) cb_(selected_); });
            panel_->add(rb);
            offsetY += 30.f;
        }
    }

    tgui::Panel::Ptr widget() const { return panel_; }
    std::string selected() const { return selected_; }
    void onSelect(Callback cb) { cb_ = std::move(cb); }

private:
    tgui::Panel::Ptr panel_;
    std::vector<Option> options_;
    std::string selected_;
    Callback cb_;
};

#endif

