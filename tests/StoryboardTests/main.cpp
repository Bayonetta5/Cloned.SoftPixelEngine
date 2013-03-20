//
// SoftPixel Engine - Storyboard Tests
//

#include <SoftPixelEngine.hpp>
#include <boost/foreach.hpp>

using namespace sp;

#ifdef SP_COMPILE_WITH_STORYBOARD

#include "../common.hpp"

SP_TESTS_DECLARE

const s32 BLOCK_RADIUS = 35;
dim::point2di Pos(400, 300);
std::vector<dim::point2di> BlockList;

class MyConsequenceA : public tool::Trigger
{
    
    public:
        
        MyConsequenceA() :
            tool::Trigger()
        {
        }
        ~MyConsequenceA()
        {
        }
        
        void onTriggered()
        {
            io::Log::message("Triggered A");
        }
        void onUntriggered()
        {
            io::Log::message("Untriggered A");
        }
        
};

class MyConsequenceB : public tool::Trigger
{
    
    public:
        
        MyConsequenceB() :
            tool::Trigger()
        {
        }
        ~MyConsequenceB()
        {
        }
        
        void onTriggered()
        {
            io::Log::message("Triggered B");
        }
        void onUntriggered()
        {
            io::Log::message("Untriggered B");
        }
        
};

class MyEventA : public tool::Event
{
    
    public:
        
        MyEventA() :
            tool::Event()
        {
        }
        ~MyEventA()
        {
        }
        
        void update()
        {
            bool Collided = false;
            
            const dim::rect2di Rect(
                Pos.X - BLOCK_RADIUS, Pos.Y - BLOCK_RADIUS,
                Pos.X + BLOCK_RADIUS, Pos.Y + BLOCK_RADIUS
            );
            
            foreach (const dim::point2di &Pnt, BlockList)
            {
                const dim::rect2di BlockRect(
                    Pnt.X - BLOCK_RADIUS, Pnt.Y - BLOCK_RADIUS,
                    Pnt.X + BLOCK_RADIUS, Pnt.Y + BLOCK_RADIUS
                );
                
                if (BlockRect.overlap(Rect))
                {
                    Collided = true;
                    break;
                }
            }
            
            if (Collided)
                trigger();
            else
                untrigger();
        }
        
};

class MyEventB : public tool::Event
{
    
    public:
        
        MyEventB() :
            tool::Event()
        {
        }
        ~MyEventB()
        {
        }
        
        void update()
        {
            if (spControl->keyDown(io::KEY_RETURN))
                trigger();
            else
                untrigger();
        }
        
};

void DrawBlock(s32 X, s32 Y, s32 Radius = BLOCK_RADIUS, const video::color &Color = 255)
{
    spRenderer->draw2DRectangle(dim::rect2di(X - Radius, Y - Radius, X + Radius, Y + Radius), Color);
}

int main()
{
    SP_TESTS_INIT("Storyboard")
    
    tool::Storyboard story;
    
    MyEventA* myEventA = story.createEvent<MyEventA>();
    MyEventB* myEventB = story.createEvent<MyEventB>();
    tool::EventTimer* myTimer = story.createEvent<tool::EventTimer>(500);
    
    tool::LogicGate* myGate1 = story.createTrigger<tool::LogicGate>(tool::LOGICGATE_AND);
    tool::LogicGate* myGate2 = story.createTrigger<tool::LogicGate>(tool::LOGICGATE_OR);
    tool::TriggerCounter* myCounter = story.createTrigger<tool::TriggerCounter>(10);
    tool::TriggerSwitch* mySwitch = story.createTrigger<tool::TriggerSwitch>(2);
    
    MyConsequenceA* myConsequenceA = story.createTrigger<MyConsequenceA>();
    MyConsequenceB* myConsequenceB = story.createTrigger<MyConsequenceB>();
    
    #if 1
    myEventA->connect(myGate2);
    myEventB->connect(myGate2);
    myGate2->connect(myGate1);
    //myTimer->connect(myGate1);
    myGate1->connect(myCounter);
    myCounter->connect(mySwitch);
    mySwitch->connect(myConsequenceA);
    mySwitch->connect(myConsequenceB);
    #else
    myTimer->connect(myConsequenceA);
    #endif
    
    const s32 Speed = 5;
    
    BlockList.push_back(dim::point2di(100, 50));
    
    
    SP_TESTS_MAIN_BEGIN
    {
        spRenderer->beginDrawing2D();
        
        if (spControl->keyDown(io::KEY_LEFT )) Pos.X -= Speed;
        if (spControl->keyDown(io::KEY_RIGHT)) Pos.X += Speed;
        if (spControl->keyDown(io::KEY_UP   )) Pos.Y -= Speed;
        if (spControl->keyDown(io::KEY_DOWN )) Pos.Y += Speed;
        
        foreach (const dim::point2di &Pnt, BlockList)
            DrawBlock(Pnt.X, Pnt.Y);
        
        story.update();
        
        DrawBlock(Pos.X, Pos.Y, 35, video::color(0, 255, 0));
        
        spRenderer->endDrawing2D();
    }
    SP_TESTS_MAIN_END
}

#else

int main()
{
    io::Log::error("This engine was not compiled with storyboard utility");
    return 0;
}

#endif
