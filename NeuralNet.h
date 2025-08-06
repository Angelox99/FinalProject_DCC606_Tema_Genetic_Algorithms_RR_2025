// NeuralNet.h
#pragma once
#include <vector>
#include <cmath>

class NeuralNet {
private:
    float w1[6][4];
    float b1[6];
    float w2[3][6];
    float b2[3];

    std::vector<float> lastInputs;
    std::vector<float> lastHidden;
    std::vector<float> lastOutputs;
    int lastDecision = 0;

    static float tanh_act(float x) { return std::tanh(x); }

public:
    NeuralNet(const std::vector<float>& params) {
        size_t idx = 0;
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 4; ++j)
                w1[i][j] = idx < params.size() ? params[idx++] : 0.0f;
        for (int i = 0; i < 6; ++i)
            b1[i] = idx < params.size() ? params[idx++] : 0.0f;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 6; ++j)
                w2[i][j] = idx < params.size() ? params[idx++] : 0.0f;
        for (int i = 0; i < 3; ++i)
            b2[i] = idx < params.size() ? params[idx++] : 0.0f;
    }
    std::vector<float> GetWeights() const {
        std::vector<float> params;
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 4; ++j)
                params.push_back(w1[i][j]);
        for (int i = 0; i < 6; ++i)
            params.push_back(b1[i]);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 6; ++j)
                params.push_back(w2[i][j]);
        for (int i = 0; i < 3; ++i)
            params.push_back(b2[i]);
        return params;
    }
    int Decide(const std::vector<float>& sensors) {
        lastInputs = sensors;
        lastHidden.assign(6, 0.0f);
        lastOutputs.assign(3, 0.0f);

        for (int i = 0; i < 6; ++i) {
            float sum = b1[i];
            for (int j = 0; j < 4; ++j)
                sum += w1[i][j] * sensors[j];
            lastHidden[i] = tanh_act(sum);
        }
        for (int i = 0; i < 3; ++i) {
            float sum = b2[i];
            for (int j = 0; j < 6; ++j)
                sum += w2[i][j] * lastHidden[j];
            lastOutputs[i] = sum;
        }
        int idx = 0;
        float maxVal = lastOutputs[0];
        for (int i = 1; i < 3; ++i) {
            if (lastOutputs[i] > maxVal) { maxVal = lastOutputs[i]; idx = i; }
        }
        lastDecision = idx;
        return idx;
    }

    const std::vector<float>& GetLastInputs() const { return lastInputs; }
    const std::vector<float>& GetLastHidden() const { return lastHidden; }
    const std::vector<float>& GetLastOutputs() const { return lastOutputs; }
    int GetLastDecision() const { return lastDecision; }
};