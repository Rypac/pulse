#pragma once

#include "pulse/extensions/Ref.hpp"
#include "pulse/views/View.hpp"

template <typename T = View, typename = typename std::enable_if<std::is_base_of<cocos2d::Node, T>::value>::type>
class ViewController {
  public:
    ViewController(): view_{nullptr} {}

    ViewController(T* view) {
        loadView(view);
    }

    virtual ~ViewController() {
        if (view_) {
            view_->release();
        }
    }

    virtual T* view() {
        if (!view_) {
            loadView();
            view_->setOnEnterCallback([this]() { this->viewWillAppear(); });
            view_->setonEnterTransitionDidFinishCallback([this]() { this->viewDidAppear(); });
            view_->setonExitTransitionDidStartCallback([this]() { this->viewWillDisappear(); });
            view_->setOnExitCallback([this]() { this->viewDidDisappear(); });
            viewDidLoad();
        }
        return view_;
    }

    virtual void viewDidLoad() {}
    virtual void viewWillAppear() {}
    virtual void viewDidAppear() {}
    virtual void viewWillDisappear() {}
    virtual void viewDidDisappear() {}

  protected:
    virtual void loadView() {
        setView(autoreleased<T>());
    }

    void setView(T* view) {
        view_ = view;
        if (view_) {
            view_->retain();
        }
    }

  private:
    T* view_;
};
