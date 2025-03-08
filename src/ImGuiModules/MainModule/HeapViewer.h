#pragma once

#include "BaseModule.h"

//#include <iostream>

namespace ImGuiModules
{
    class HeapViewer : public ImGuiModules::BaseModule
    {
    private:
        //wk::mem::cSystemHeap* sysHeapPtr;
        wk::mem::cHeap* entityHeap;
        //wk::math::cVec testVec;
    public:
        HeapViewer()
        {

        };

        void Main()
        {
            if (!m_IsShown)
                return;
                        
            //std::cout << wk::math::cVec::GetLength(testVec) << std::endl;


            entityHeap = (wk::mem::cHeap*) (mainModuleBase + 0x9c6a90);

            ImGui::Begin("Heap Viewer", &m_IsShown);

            ImGui::Text("Default Heap Pointer: %p", wk::mem::MemDefaultHeapPtr);

            ImGui::Text("SystemHeap: %p", &wk::mem::SystemHeap);

            if (ImGui::Button("Clear Defauly Heap")) wk::mem::MemDefaultHeapPtr->clear();

            if (ImGui::Button("Clear Entity Heap")) entityHeap->clear();

            //if (ImGui::Button("Begin Debug Test"))
            //    test::init(100, 100);
            
            ImGui::End();
        }
    };
}