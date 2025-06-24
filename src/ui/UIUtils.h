#ifndef UIUTILS_H
#define UIUTILS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>

#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

class CometaUIUtils{

  public:
    static void ShowMat4(const glm::mat4& matrix, const char* label = "Matrix 4x4", bool tree_node = true) {
        if (tree_node) {
            if (!ImGui::TreeNode(label)) return;
        } else {
            ImGui::Text("%s", label);
        }

        if (ImGui::BeginTable("Mat4Table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            // GLM matrices are column-major by default
            for (int row = 0; row < 4; row++) {
                ImGui::TableNextRow();
                for (int col = 0; col < 4; col++) {
                    ImGui::TableSetColumnIndex(col);
                    ImGui::Text("% .4f", matrix[col][row]); // Note: column first
                }
            }
            ImGui::EndTable();
        }

        if (tree_node) {
            ImGui::TreePop();
        }
    }

    static void ShowMat3(const glm::mat3& matrix, const char* label = "Matrix 3x3", bool tree_node = true) {
        if (tree_node) {
            if (!ImGui::TreeNode(label)) return;
        } else {
            ImGui::Text("%s", label);
        }

        if (ImGui::BeginTable("Mat3Table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            for (int row = 0; row < 3; row++) {
                ImGui::TableNextRow();
                for (int col = 0; col < 3; col++) {
                    ImGui::TableSetColumnIndex(col);
                    ImGui::Text("% .4f", matrix[col][row]); // Column-major access
                }
            }
            ImGui::EndTable();
        }

        if (tree_node) {
            ImGui::TreePop();
        }
    }
};

#endif //UIUTILS_H
