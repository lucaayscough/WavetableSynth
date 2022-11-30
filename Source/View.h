#pragma once


class View : juce::OpenGLAppComponent
{
public:
    View();
    ~View();
    
    void initialise() override;
    void shutdown() override;
    void render() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (View)
};
