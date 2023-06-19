#pragma once

#include "BaseModule.h"

//#include <iostream>

namespace ImGuiModules
{
    class HeapViewer : public ImGuiModules::BaseModule
    {
    private:
        wk::mem::cHeap* test;
        //wk::math::cVec testVec;
    public:
        HeapViewer()
        {

        };

        void Main()
        {
            if (!m_IsShown)
                return;

            test = wk::mem::MemDefaultHeapPtr;

            //std::cout << wk::math::cVec::GetLength(testVec) << std::endl;

            ImGui::Begin("Heap Viewer", &m_IsShown);

            ImGui::Text("Default Heap Pointer: %p", test);


            //if (ImGui::Button("Begin Debug Test"))
            //    test::init(100, 100);
            
            ImGui::End();
        }
    };
}