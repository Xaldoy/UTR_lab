#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <functional>

class DeltaTransition
{
public:
    std::string input, fromState;
    std::set<std::string> toStates;
    DeltaTransition(const std::string &_input, const std::string &_fromState, const std::set<std::string> &_toStates) : input(_input), fromState(_fromState), toStates(_toStates) {}
};

class ENKA
{
private:
    std::set<std::string> states;
    std::set<std::string> alphabet;
    std::set<std::string> acceptableStates;
    std::string initialState;
    std::vector<DeltaTransition> deltaTransitions;

    std::set<std::string> findEpsilonEnvironment(const std::set<std::string> &states)
    {
        std::set<std::string> epsilonEnvironment;
        for (const auto &state : states)
        {
            std::set<std::string> localEpsilonEnv;
            findEpsilonRecursive(state, localEpsilonEnv);
            epsilonEnvironment.insert(localEpsilonEnv.begin(), localEpsilonEnv.end());
        }
        return epsilonEnvironment;
    }

    void findEpsilonRecursive(const std::string &state, std::set<std::string> &epsilonEnvironment)
    {
        epsilonEnvironment.insert(state);
        for (const auto &transition : deltaTransitions)
        {
            if (transition.fromState == state && transition.input == "$")
            {
                for (const auto &nextState : transition.toStates)
                {
                    if (epsilonEnvironment.find(nextState) == epsilonEnvironment.end())
                    {
                        findEpsilonRecursive(nextState, epsilonEnvironment);
                    }
                }
            }
        }
    }

    std::set<std::string> deltaTransition(const std::set<std::string> &states, const std::string &input)
    {
        std::set<std::string> result;
        for (const auto &state : states)
        {
            for (const auto &transition : deltaTransitions)
            {
                if (transition.fromState == state && transition.input == input)
                {
                    result.insert(transition.toStates.begin(), transition.toStates.end());
                }
            }
        }
        if (result.empty())
            result.insert("#");
        return result;
    }

    void printState(std::set<std::string> &result)
    {
        bool printComma = false;
        for (const auto &line : result)
        {
            if (line == "#" && result.size() != 1)
                continue;
            if (printComma)
                std::cout << ",";
            else
                printComma = true;
            std::cout << line;
        }
    }

public:
    ENKA(
        const std::set<std::string> &states_,
        const std::set<std::string> &alphabet_,
        const std::set<std::string> &acceptableStates_,
        const std::string &initialState_,
        const std::vector<DeltaTransition> &deltaTransitions_) : states(states_), alphabet(alphabet_),
                                                                 acceptableStates(acceptableStates_), initialState(initialState_),
                                                                 deltaTransitions(deltaTransitions_)
    {
    }

    void simulate(const std::vector<std::string> &inputAlphabet)
    {
        std::set<std::string> currentStates = findEpsilonEnvironment({initialState});
        printState(currentStates);
        for (const auto &input : inputAlphabet)
        {
            std::cout << "|";
            std::set<std::string> result = findEpsilonEnvironment(deltaTransition(findEpsilonEnvironment(currentStates), input));
            currentStates = result;
            printState(result);
        }
    }
};

std::vector<std::string> tokeniseByDelim(const std::string input, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delim))
        result.push_back(token);

    return result;
}

DeltaTransition createDeltaTransition(const std::string &transition)
{
    std::stringstream ss(transition);
    std::string input, fromState, toStatesString;

    std::getline(ss, fromState, ',');
    std::getline(ss, input, '-');
    std::getline(ss, toStatesString);

    std::vector<std::string> temp = tokeniseByDelim(toStatesString.substr(1), ',');

    std::set<std::string> toStates;

    toStates.insert(temp.begin(), temp.end());

    DeltaTransition deltaTransition = DeltaTransition(input, fromState, toStates);
    return deltaTransition;
}

std::set<std::string> getSetFromInput(const std::string &input)
{
    std::vector<std::string> tokenizedVector = tokeniseByDelim(input, ',');
    std::set<std::string> result;
    result.insert(tokenizedVector.begin(), tokenizedVector.end());
    return result;
}

int main()
{
    std::vector<std::string> multiInput;
    std::set<std::string> states, alphabet, acceptableStates, transitionFunctions;
    std::string initialState, input, line;

    std::getline(std::cin, input);
    multiInput = tokeniseByDelim(input, '|');

    std::getline(std::cin, input);
    states = getSetFromInput(input);

    std::getline(std::cin, input);
    alphabet = getSetFromInput(input);

    std::getline(std::cin, input);
    acceptableStates = getSetFromInput(input);

    std::getline(std::cin, input);
    initialState = input;

    while (std::getline(std::cin, line))
    {
        transitionFunctions.insert(line);
    }

    std::vector<DeltaTransition> deltaTransitions;

    for (const auto &transition : transitionFunctions)
    {
        deltaTransitions.push_back(createDeltaTransition(transition));
    }

    ENKA enka(states, alphabet, acceptableStates, initialState, deltaTransitions);

    for (const auto &input : multiInput)
    {
        std::vector<std::string> tokenizedInput = tokeniseByDelim(input, ',');
        enka.simulate(tokenizedInput);
        std::cout << std::endl;
    }

    return 0;
}