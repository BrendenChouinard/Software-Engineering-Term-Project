#ifndef INTEGER_ENTRY_H
#define INTEGER_ENTRY_H

#include <TGUI/Widgets/EditBox.hpp>
#include <functional>
#include <string>

class IntegerEntry {
public:
    using Callback = std::function<void(int,bool)>;

    IntegerEntry() {
        box_ = tgui::EditBox::create();
        box_->setSize(100, 24);
        box_->onTextChange([this]{ validate(); });
    }

    void setPosition(float x, float y) { box_->setPosition(x,y); }
    tgui::EditBox::Ptr widget() const { return box_; }

    int value() const { return value_; }
    bool valid() const { return valid_; }

    void onChange(Callback cb) { cb_ = std::move(cb); }

private:
    void validate() {
        std::string raw = box_->getText().toStdString();
        if (raw.empty()) { valid_ = false; box_->getRenderer()->setTextColor(tgui::Color::White); if (cb_) cb_(0,false); return; }
        try {
            size_t pos = 0; int v = std::stoi(raw,&pos); if (pos != raw.size()) throw std::runtime_error("partial");
            value_ = v; valid_ = true; box_->getRenderer()->setTextColor(tgui::Color::Green);
        } catch(...) {
            valid_ = false; box_->getRenderer()->setTextColor(tgui::Color::Red);
        }
        if (cb_) cb_(value_, valid_);
    }
    tgui::EditBox::Ptr box_;
    int value_ = 0;
    bool valid_ = false;
    Callback cb_;
};

#endif

