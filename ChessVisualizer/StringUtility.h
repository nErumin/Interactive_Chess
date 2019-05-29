#ifndef STRINGUTILITY_H
#define STRINGUTILITY_H

#include <vector>
#include <string>
#include <sstream>
#include <Vector2.h>

using Difference = std::pair<Vector2, Vector2>;

std::string formatLocationsToProtocol(const std::vector<Difference>& locationDifferences)
{
    std::ostringstream protocolStream;

    protocolStream << locationDifferences.size() << ":";

    for (const auto& difference : locationDifferences)
    {
        auto before = normalizeToIntegerVector(difference.first);
        auto after = normalizeToIntegerVector(difference.second);

        protocolStream << before.first << "," << before.second << "$"
                       << after.first << "," << after.second << "|";
    }

    auto protocolMessage = protocolStream.str();

    // Remove last delimeter.
    return protocolMessage.substr(0, protocolMessage.size() - 1);
}

auto splitString(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> brokenString;
    size_t searchStartIndex = 0;

    while (searchStartIndex < str.size())
    {
        size_t foundIndex = str.find_first_of(delimiter, searchStartIndex);

        if (foundIndex == std::string::npos)
        {
            foundIndex = str.size() + searchStartIndex;
        }

        brokenString.push_back(str.substr(searchStartIndex, foundIndex - searchStartIndex));
        searchStartIndex = foundIndex + delimiter.size();
    }

    return brokenString;
}

std::vector<Difference> parseDifferenceProtocolMessage(const std::string& message)
{
    auto countSplitMessage = splitString(message, ":");
    size_t diffCount = std::stoul(countSplitMessage.at(0));

    std::vector<Difference> parsedResult;

    if (diffCount > 0)
    {
        auto differences = splitString(countSplitMessage.at(1), "|");

        for (size_t index = 0; index < diffCount; ++index)
        {
            auto beforeAfter = splitString(differences[index], "$");
            auto before = splitString(beforeAfter.at(0), ",");
            auto after = splitString(beforeAfter.at(1), ",");

            parsedResult.emplace_back(
                Vector2(std::stoi(before.at(0)), std::stoi(before.at(1))),
                Vector2(std::stoi(after.at(0)), std::stoi(after.at(1)))
            );
        }
    }

    return parsedResult;
}

#endif // STRINGUTILITY_H
