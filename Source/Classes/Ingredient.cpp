#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Ingredient.h"

// Required by the nlohmann/json library for C++20 compatibility when compiling.
#define JSON_HAS_CPP_20 0
#include "json.hpp"  // Includes the popular nlohmann/json library for parsing and generating JSON data.
#include "axmol.h"   // Includes the core axmol game engine library, likely for file utility functions.

// Vector to hold all Ingredient objects from the ingredients.json file.
std::vector<Ingredient> Ingredient::ingredients;

// Constructors

// Default constructor: Initializes an Ingredient object with empty or zero values.
Ingredient::Ingredient()
{
    name      = "";
    amount    = 0;
    cost      = 0;
    quantity  = 0;
    type      = -1;
    nameIndex = -1;
}

// Constructor: Initializes an Ingredient with a name, a base amount, and a unit.
// Other properties (cost, quantity, type, nameIndex) are set to default.
Ingredient::Ingredient(std::string n, int a, std::string u)
{
    name      = n;
    amount    = a;
    unit      = u;
    cost      = 0;
    quantity  = 0;
    type      = -1;
    nameIndex = -1;
}

// Full constructor: Initializes an Ingredient with all its core properties.
// Quantity is implicitly set to 0 as it's typically managed separately (e.g., in inventory/cart).
Ingredient::Ingredient(std::string n, std::string u, double c, int a, int t, int i)
{
    name      = n;
    cost      = c;
    amount    = a;
    type      = t;
    quantity  = 0;
    unit      = u;
    nameIndex = i;
}

// Copy constructor: Creates a new Ingredient object from an existing one,
// allowing a specific 'quantity' to be set for the new instance.
Ingredient::Ingredient(Ingredient& i, int q)
{
    name      = i.name;
    cost      = i.cost;
    amount    = i.amount;
    type      = i.type;
    unit      = i.unit;
    quantity  = q;
    nameIndex = i.nameIndex;
}

// Getters

std::string Ingredient::getName()
{
    return name;
}

double Ingredient::getCost()
{
    return cost;
}

int Ingredient::getQuantity()
{
    return quantity;
}

int Ingredient::getAmount()
{
    return amount;
}

int Ingredient::getNameIndex()
{
    return nameIndex;
}

// Static getter: Returns a reference to the global list of all loaded ingredients.
std::vector<Ingredient> Ingredient::getIngredients()
{
    return ingredients;
}

// Setters

void Ingredient::setQuantity(int q)
{
    quantity = q;
}

void Ingredient::changeQuantityBy(int q)
{
    quantity += q;
}

void Ingredient::setNameIndex(int i)
{
    nameIndex = i;
}

// --- Overloaded operators for Ingredient class ---

// Overloaded assignment operator: Enables direct assignment between Ingredient objects,
// copying all member values.
void Ingredient::operator=(const Ingredient& i)
{
    name      = i.name;
    cost      = i.cost;
    amount    = i.amount;
    type      = i.type;
    unit      = i.unit;
    quantity  = i.quantity;
    nameIndex = i.nameIndex;
}

// --- Static method to load ingredients from JSON file ---

// Populates the static 'ingredients' vector by reading data from "ingredients.json".
void Ingredient::getIngredientList()
{
    // Get the full system path to the "ingredients.json" file using Axmol's resource manager.
    std::string fullPath = ax::FileUtils::getInstance()->fullPathForFilename("ingredients.json");
    std::ifstream file(fullPath);  // Open the file for reading.

    // Print the current working directory for debugging purposes (useful for verifying file location).
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    // Check if the file stream is successfully opened. If not, print an error and exit the function.
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << fullPath << std::endl;
        return;
    }

    std::cout << "Reading ingredients from JSON file..." << std::endl;

    try
    {
        nlohmann::json json_data;  // Create a JSON object to hold the parsed file content.
        file >> json_data;         // Parse the entire content of the file into the JSON object.

        int index = 0;  // Initialize an index to assign to each ingredient, reflecting its order in the JSON.
        // Loop through each JSON object (representing an ingredient) in the parsed JSON array.
        for (const auto& item : json_data)
        {
            // Create a new Ingredient object using data extracted from the current JSON item.
            // The 'index' is passed to store its position in the original JSON file.
            Ingredient ingredient(item["name"], item["unit"], item["cost"], item["amount"], item["type"], index);
            // Add the newly created Ingredient object to the static 'ingredients' vector.
            Ingredient::ingredients.push_back(ingredient);

            index++;  // Increment the index for the next ingredient.

            // Debugging output: Print details of each ingredient as it is successfully loaded.
            std::cout << "Loaded Ingredient: " << ingredient.name << ", Cost: " << ingredient.cost
                      << ", Amount: " << ingredient.amount << " " << ingredient.unit << ", Type: " << ingredient.type
                      << ", Index: " << ingredient.nameIndex << std::endl;
        }
    }
    catch (const nlohmann::json::parse_error& e)
    {
        // Catch and report errors specifically related to parsing malformed JSON data.
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        // Catch any other standard C++ exceptions that might occur during the process.
        std::cerr << "General error: " << e.what() << std::endl;
    }
}
