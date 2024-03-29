#pragma once
#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QMap>
#include <QString>

enum SpeciesType {
   HUMAN     = 0,
   CARIBLIN  = 1,
   LITTER    = 2,
   FEXEL     = 3,
   KROCKER   = 4,
   GOBBLEDEW = 5,
   BAD_SPECIES
};

static QString GetSpeciesName(SpeciesType species)
{
   QString name = "";
   switch (species)
   {
   case HUMAN:
      name = "HUMAN";
      break;
   case CARIBLIN:
      name = "CARIBLIN";
      break;
   case LITTER:
      name = "LITTER";
      break;
   case FEXEL:
      name = "FEXEL";
      break;
   case KROCKER:
      name = "KROCKER";
      break;
   case GOBBLEDEW:
      name = "GOBBLEDEW";
      break;
   default:
      name = "Bad Species";
   }
   return name;
}

static SpeciesType GetSpeciesType(QString speciesName)
{
   SpeciesType species = BAD_SPECIES;
   if (QString::compare("HUMAN", speciesName, Qt::CaseInsensitive) == 0)
   {
      species = HUMAN;
   }
   else if (QString::compare("CARIBLIN", speciesName, Qt::CaseInsensitive) == 0)
   {
      species = CARIBLIN;
   }
   else if (QString::compare("LITTER", speciesName, Qt::CaseInsensitive) == 0)
   {
      species = LITTER;
   }
   else if (QString::compare("FEXEL", speciesName, Qt::CaseInsensitive) == 0)
   {
      species = FEXEL;
   }
   else if (QString::compare("KROCKER", speciesName, Qt::CaseInsensitive) == 0)
   {
      species = KROCKER;
   }
   else if (QString::compare("GOBBLEDEW", speciesName, Qt::CaseInsensitive) == 0)
   {
      species = GOBBLEDEW;
   }
   return species;
}

struct SpeciesRelationship
{
private:
   SpeciesType speciesA;
   SpeciesType speciesB;
   double affinity;
   bool itemChanged = false;
public:
   SpeciesRelationship(SpeciesType speciesA, SpeciesType speciesB, double affinity)
   {
      this->speciesA = speciesA;
      this->speciesB = speciesB;
      this->affinity = affinity;
   }
   bool DidChange() { return itemChanged; }
   double GetSpeciesAffinity() { return this->affinity; }
   void SetAffinity(double affinity) 
   {
      itemChanged = true;
      if (affinity < -1.0)
      {
         this->affinity = -1.0;
      }
      else if (affinity > 1.0)
      {
         this->affinity = 1.0;
      }
      else
      {
         this->affinity = affinity;
      }
   }

   SpeciesType GetSpeciesA() { return this->speciesA; }
   SpeciesType GetSpeciesB() { return this->speciesB; }
};

enum PineItemType {
   FOOD,                //Food items.
   MATERIAL,            //Materials used for crafting.
   ESSENCE,             //Gained from killing creatures. Used in crafting.
   TOKEN,               //Given by a village leader to perform a task.
   CRAFTED,             //Crafted items.
   MIXTURE,             //Crafted drinks.
   TRAP,                //Traps for enemies.
   RAID_FLARE,          //Summons help for raiding.
   KEYS,                //Unlocks things.
   SIMULATION,          //Items that change the simulation or NPCs in some way.
   AMMUNITION,          //Ammunition for bows.
   EQUIPMENT,           //Equipable items.
   IDEA,                //Gives the ability to craft something.
   IMPORTANT,           //Unique items that can be obtained once.
   OTHER,               //The spaces for Amphiscus Orbs, Key Graphite and such.
   SORT_OF_ITEM,        //Items that have an icon but the name is just 'Item Name'.
   GAME_BREAKING_ITEM,  //Causes save file to not load.
   NO_ITEM              //Doesn't break the game but doesn't show an item.
};

static QMap<PineItemType, QString> itemNameMap {
   { FOOD, "Food" },
   { MATERIAL, "Material" },
   { ESSENCE, "Essence" },
   { TOKEN, "Token" },
   { CRAFTED, "Crafted" },
   { MIXTURE, "Mixture" },
   { TRAP, "Trap" },
   { RAID_FLARE, "Raid Flare" },
   { KEYS, "Keys" },
   { SIMULATION, "Simulation" },
   { AMMUNITION, "Ammunition" },
   { EQUIPMENT, "Equipment" },
   { IDEA, "Idea" },
   { IMPORTANT, "Important" },
   { OTHER, "Other" },
   { SORT_OF_ITEM, "Sort Of Item" },
   { GAME_BREAKING_ITEM, "Game Breaker" },
   { NO_ITEM, "Invisible" }
};

struct PineItem {
   int itemId = 0;
   int maxStack = 0;
   PineItemType itemType = NO_ITEM;
   QString itemName = "";
   QString itemDescription = "";

   bool itemChanged = false;
};

struct PlayerItem
{
   PineItem item;
   int count;
};

static const QList<PineItem> pineItemList = {
   { 0, 15, FOOD, "Roseberry", "A common, tiny sweet fruit that grows on specific bushes in the Wedgewoods." },
   { 1, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 2, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 3, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 4, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 5, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 6, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 7, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 8, 10, FOOD, "Carrant", "A healthy, small vegetable growing on tall plants. Somewhat energizing, found in foresty areas." },
   { 9, 10, FOOD, "Meageryam", "An edible, not very tasty vegetable that grows low on the ground in and around the Wedgewoods." },
   { 10, 20, FOOD, "Tingflower", "A common, edible flower that grows in most fields on Albamare." },
   { 11, 20, FOOD, "Commonwheat", "A common and important cereal eaten by most species of Albamare, found on its lower plains." },
   { 12, 15, FOOD, "Alpafant Meat", "A sturdy piece of fresh meat from an Alpafant Critter." },
   { 13, 15, FOOD, "Puffle Egg", "A small, pink egg from a Puffle critter." },
   { 14, 10, MATERIAL, "Marrwood", "A small log, easy to process for crafting a variety of tools." },
   { 15, 10, CRAFTED, "Stiffrope", "A crafted, braided thread used to bind objects together when crafting." },
   { 16, 5, MATERIAL, "Lean-Iron", "A common, chemically sturdy material to solidify mostly anything with. Can be found against cliffs, mainly in the mountain areas." },
   { 17, 10, ESSENCE, "Cariblin Essence", "A soul-like fragment of a Cariblin organism" },
   { 18, 10, ESSENCE, "Litter Essence", "A soul-like fragment of a Litter organism." },
   { 19, 10, MATERIAL, "Dullrock", "A common type of rock used for crafting a wide variety of tools." },
   { 20, 10, ESSENCE, "Krocker Essence", "A soul-like fragment of a Krocker organism." },
   { 21, 10, ESSENCE, "Fexel Essence", "A soul-like fragment of a Fexel organism." },
   { 22, 15, MATERIAL, "Crassbone", "Marrow and bone from deceased species and critters." },
   { 23, 10, CRAFTED, "Stuffcloth", "Crafted, soft, woven fabric to cover objects when crafting." },
   { 24, 20, MATERIAL, "Puffle Feather", "A small pink feather from the rear end of a Puffle critter." },
   { 25, 10, MATERIAL, "Alpafant Leather", "A thin, grey-ish leather that can be used for isolation and coverage, from Albamare's Alpafant critters." },
   { 26, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 27, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 28, 1, IMPORTANT, "Amam's Map", "A map drawn by Amam while preparing to explore the world around the Unstable Cliffs." },
   { 29, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 30, 1, IMPORTANT, "Conducting Lantern", "An Outfinding of an earlier civilization that energizes even the darkest of places." },
   { 31, 1, IMPORTANT, "Scouting Lens", "An Outfinding of an earlier civilization that lets set beyond what you know." },
   { 32, 1, IMPORTANT, "Vault Key", "A large key that opens the large gates to Vaults." },
   { 33, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 34, 10, MATERIAL, "Sandstone", "A soft stone that crumbles on pressure, but can be very useful in crafting. Can be found on beaches and in sand." },
   { 35, 5, FOOD, "Eden Fruit", "A rare, highly energizing piece of fruit that grows on only one tree on Albamare." },
   { 36, 1, IMPORTANT, "Taming Bands", "An Outfinding of an earlier civilization that allows you to tame critters." },
   { 37, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 38, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 39, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 40, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 41, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 42, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 43, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 44, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 45, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 46, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 47, 10, CRAFTED, "Softglass", "A crafted material that is surprisingly strong and useful for see-through objects." },
   { 48, 5, MATERIAL, "Spystal", "Extremely strong material emitting a faint blue light, found between the grasses of Albamare's plains." },
   { 49, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 50, 1, IMPORTANT, "Mannoth Callus", "Extremely hard to get, but tougher than anything on Albamare." },
   { 51, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 52, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 53, 1, IMPORTANT, "Adarna Wing", "A rare wing obtained from a rare Adarna critter. Glowing red colors that make materials stronger." },
   { 54, 1, IMPORTANT, "Lumiconda Tooth", "A strong material of an ancient snake-like critter that once roamed Albamare." },
   { 55, 10, MATERIAL, "Toothstone", "A hardened plaque of dental bacteria of larger species and critters, found in swampy areas." },
   { 56, 5, MATERIAL, "Slick Pearl", "A costly, shiny pearl, appearing in only some of the shells that wash up on Albamare's shores." },
   { 57, 15, MATERIAL, "Lunarodos", "A bright flower, found mainly in soggy areas, of which the extract can be applied to solidify and enhance." },
   { 58, 15, MATERIAL, "Solfodil", "A dark flower, found mainly in mountainous areas, of which the extract is used to enhance many objects." },
   { 59, 20, FOOD, "Dunerice", "An energizing, edible seed of a grass that grows wildly in the Dune areas of Albamare." },
   { 60, 15, FOOD, "Nuctus", "A sweet, hard and nutritious piece of food that grows on tough green plants in the dry parts of Albamare." },
   { 61, 10, FOOD, "Fatplant", "A rather heavy, edible plant that holds plenty of fresh water, found in warm, dry areas." },
   { 62, 5, FOOD, "Leafdough", "A fabricated, energizing paste of wheat and bush leaves used by some species to spice up their dinner." },
   { 63, 10, FOOD, "Waddletooth Loin", "A squiggly piece of loin from a Waddletooth critter. Disgusting, but nutritious." },
   { 64, 10, FOOD, "Bleeker Thigh", "A rare and rather slimey piece of Bleeker meat." },
   { 65, 1, SORT_OF_ITEM, "Item Name (Looks like meat)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 66, 10, FOOD, "Mudbeet", "A very nutritious, bitter beet eaten by most creatures in the Soggy Woodlands." },
   { 67, 10, FOOD, "Anura Shroom", "A rather slimey mushroom type, traditionally eaten by slimey creatures only, now widely found around the soggy arreas of Albamare." },
   { 68, 15, FOOD, "Obergine", "A long, bitter-sweet plant full of edible fluids that is found low in the ground in swamps." },
   { 69, 15, FOOD, "Avian Pepper", "A highly energizing, spicy vegetable originating from the Arid Canyon." },
   { 70, 20, FOOD, "Ridge Fennel", "A nutritious, shot type of grain growing on ridges in the mountain area." },
   { 71, 20, FOOD, "Telkin Chives", "A bright green food that keeps your feet warm and energizes your movements, found in the higher regions of Albamare." },
   { 72, 3, MATERIAL, "Beagalite", "A strong mineral unique to Albamare, highly valued and used to solidify almost any object." },
   { 73, 10, ESSENCE, "Gobbledew Essence", "A soul-like fragment of a Gobbledew organism." },
   { 74, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 75, 15, MATERIAL, "Gravel Moss", "Moss that grows on low rocks in and around the Wedgewoods." },
   { 76, 15, MATERIAL, "Grandcone", "Cones of the common conifers of the Wedgewoods. Finding them near large logs." },
   { 77, 15, MATERIAL, "Morrow Hay", "A soft grass that, when bound, creates strong structures. Finding this in lower grassy areas." },
   { 78, 5, MATERIAL, "Dry Clay", "A sturdy building material made of the iconic red rock of the dunes of Albamare." },
   { 79, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 80, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 81, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 82, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 83, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 84, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 85, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 86, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 87, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 88, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 89, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 90, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 91, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 92, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 93, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 94, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 95, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 96, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 97, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 98, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 99, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 100, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 101, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 102, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 103, 50, MIXTURE, "Spurt Mixture", "A mixture that makes you move faster." },
   { 104, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 105, 50, MIXTURE, "Racer Drink", "A mixture that lets you sprint for a longer time." },
   { 106, 50, MIXTURE, "Hop Brew", "A mixture that lets you jump higher." },
   { 107, 50, MIXTURE, "Evasion Mixture", "A mixture that lets you dodge faster." },
   { 108, 50, MIXTURE, "Clash Mixture", "A mixture that makes your melee attacks faster." },
   { 109, 50, MIXTURE, "Trigger Brew", "A mixture that makes your ranged attacks faster." },
   { 110, 50, MIXTURE, "Sly Draft", "A mixture that makes you harder to spot." },
   { 111, 50, MIXTURE, "Guardian's Mixture", "A mixture that increases your shield's stamina." },
   { 112, 50, MIXTURE, "Power Brew", "A mixture that makes your melee attacks more powerful." },
   { 113, 50, MIXTURE, "Scope Drink", "A mixture that makes your ranged attacks more powerful." },
   { 114, 50, MIXTURE, "Tenacity Mixture", "A mixture that makes you less vulnerable." },
   { 115, 50, MIXTURE, "Charisma Potion", "A mixture that makes you liked by species more easily." },
   { 116, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 117, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 118, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 119, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 120, 50, MIXTURE, "Stamina Brew", "A mixture that completely energizes you immediately." },
   { 121, 50, MIXTURE, "Vigor Brew", "A mixture that gives you 100% of your vitality back." },
   { 122, 1, IDEA, "Spurt Mixture Idea", "" },
   { 123, 1, IDEA, "Racer Drink Idea", "" },
   { 124, 1, IDEA, "Hop Brew Idea", "" },
   { 125, 1, IDEA, "Evasion Mixture Idea", "" },
   { 126, 1, IDEA, "Clash Mixture Idea", "" },
   { 127, 1, IDEA, "Trigger Brew Idea", "" },
   { 128, 1, IDEA, "Sly Draft Idea", "" },
   { 129, 1, IDEA, "Guardian's Mixture Idea", "" },
   { 130, 1, IDEA, "Power Brew Idea", "" },
   { 131, 1, IDEA, "Scope Drink Idea", "" },
   { 132, 1, IDEA, "Tenacity Mixture Idea", "" },
   { 133, 1, IDEA, "Charisma Potion Idea", "" },
   { 134, 1, IDEA, "Gobbledew Melange Idea", "" },
   { 135, 1, IDEA, "Krocker Brew Idea", "" },
   { 136, 1, IDEA, "Cariblin Potion Idea", "" },
   { 137, 1, IDEA, "Litter Broth Idea", "" },
   { 138, 1, IDEA, "Stamina Brew Idea", "" },
   { 139, 1, IDEA, "Vigor Brew Idea", "" },
   { 140, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 141, 1, IDEA, "Attic Key Idea", "" },
   { 142, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 143, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 144, 1, IMPORTANT, "Attic Key", "A key for the attic door in the Ancient Stables Vault." },
   { 145, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 146, 1, EQUIPMENT, "Gatherer's Tunic", "Basic but trustworthy clothing for gatherers and Humans living in trees." },
   { 147, 1, EQUIPMENT, "Gatherer's Slacks", "A basic pair of trousers for simple protection." },
   { 148, 1, EQUIPMENT, "Gatherer's Wrist Rope", "A tight rope around the wrist to make being around trees less straining." },
   { 149, 1, EQUIPMENT, "Wedgewood Tunic", "A moss and wood-based outfit that protects against friction and basic impacts." },
   { 150, 1, EQUIPMENT, "Wedgewood Leg Guards", "Simple wooden protectors to traverse the woods more easily." },
   { 151, 1, EQUIPMENT, "Outgoer's Arm Guards", "Solid arm protectors to somewhat protect against incoming attacks." },
   { 152, 1, EQUIPMENT, "Outgoer's Jacket", "A well-made vest that warms when it needs to and slightly protects in combat." },
   { 153, 1, EQUIPMENT, "Outgoer's Trousers", "Flexible pair of pants for when you traverse longer distances or find yourself in a conflict often." },
   { 154, 1, EQUIPMENT, "Outgoer's Shinguards", "Protective set of thick guards around the shins, to make traversing and fighting less straining." },
   { 155, 1, EQUIPMENT, "Staker's Arm Guards", "Solid arm protectors to somewhat protect against incoming attacks. It has a tinge of support to it." },
   { 156, 1, EQUIPMENT, "Staker's Jacket", "A well-made vest that warms when it needs to and slightly protects in combat. It has a tinge of support to it." },
   { 157, 1, EQUIPMENT, "Staker's Trousers", "Flexible pair of pants for when you traverse longer distances or find yourself in conflict often. It has a tinge of support to it." },
   { 158, 1, EQUIPMENT, "Staker's Shinguards", "Protective set of thick guards around the shins, to make traversing and fighting less straining. It has a tinge of support to it." },
   { 159, 1, EQUIPMENT, "Hunter's Vest", "A thick tunic ideal for battling, heavy lifting and long journeys." },
   { 160, 1, EQUIPMENT, "Hunter's Pants", "A comfortable pair of slacks that thoroughly protects the traveler wearing it." },
   { 161, 1, EQUIPMENT, "Hunter's Boots", "A strong pair of leather boots, protecting against low blows and onerous travels." },
   { 162, 1, EQUIPMENT, "Hunter's Arm Guards", "A durable set of arm guards to aid in any confrontation." },
   { 163, 1, EQUIPMENT, "Nomad's Scarf", "A light scarf to protect against tension, friction and combat, even in a stinging sun." },
   { 164, 1, EQUIPMENT, "Nomad Sleeves", "Pair of light sleeves to slightly cushion against attacks and friction." },
   { 165, 1, EQUIPMENT, "Nomad Pants", "A breezy pair of pants, perfect for in arid climates." },
   { 166, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 167, 1, EQUIPMENT, "Telkin Cap", "A large helmet, invented and crafted by Mannoth species to heavily protect even in colder areas." },
   { 168, 1, EQUIPMENT, "Telkin Gloves", "Strong set of gloves to aid you in battle or while clambering through the coldest of areas." },
   { 169, 1, EQUIPMENT, "Telkin Boots", "Soft, insulating boots to keep the feet warm and protect to a medium extent." },
   { 170, 1, EQUIPMENT, "Antler Mask", "Strong head protection crafted by the Cariblin species." },
   { 171, 1, EQUIPMENT, "Antler Braces", "Strong braces conjured up by Cariblin species, to aid you in battle and to keep you warm." },
   { 172, 1, EQUIPMENT, "Antler Shinguards", "Protective leg plates to help you hold off low-targeted attacks, designed by Cariblins." },
   { 173, 1, EQUIPMENT, "Spitter's Hood", "A tall red hood fabricated by the Litter species." },
   { 174, 1, EQUIPMENT, "Spitter's Wristguards", "Soft Litter-cloth cushions around the wrists that enhances your aegis." },
   { 175, 1, EQUIPMENT, "Spitter's Legs", "Pair of provisional leg cushions that slightly fortifies your outfit, crafted by Litters." },
   { 176, 1, EQUIPMENT, "Barrel Mask", "An intriguing mask of Lean-iron and Stuffcloth, devised by Fexel species." },
   { 177, 1, EQUIPMENT, "Barrel Gloves", "A solid pair of mitts used by Fexels in alchemy and combat." },
   { 178, 1, EQUIPMENT, "Barrel Boots", "Thick footwear made for the Fexel's delicate feet, slightly adding to your protective covering." },
   { 179, 1, EQUIPMENT, "Skull Horns", "Spiritual headwear crafted by Krocker species." },
   { 180, 1, EQUIPMENT, "Skull Arm Braces", "A strong, protective splint around the arms to fortify one's stability, invented by Krocker species." },
   { 181, 1, EQUIPMENT, "Skull Leg Braces", "A set of durable splints near the legs to improve stability, made by Krockers." },
   { 182, 1, EQUIPMENT, "Feather Adornment", "An alluring set of feathers arranged for on the head by Gobbledew species." },
   { 183, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 184, 1, EQUIPMENT, "Feather Sleeves", "Adorning, delicate sleeves to enhance protection, created by Gobbledew." },
   { 185, 1, EQUIPMENT, "Feather Shinguards", "Elegant armament around the legs to improve one's armor, crafted by Gobbledew." },
   { 186, 15, MATERIAL, "Waddletooth Blubber", "A jelly-like substance from Waddletooths, secreted when they feel at home." },
   { 187, 99, OTHER, "Key Graphite", "A dark grit which, when compressed, forms intricately shaped and solid items." },
   { 188, 15, MATERIAL, "Bleeker Antenna", "A slimey antenna of a deceased Bleeker critter, which still seems to hold some of its conducting capabilities." },
   { 189, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 190, 1000, OTHER, "Albamare Key", "One of Albamare's unique keys, opening its unique chests." },
   { 191, 1, IDEA, "Stiffrope Idea", "" },
   { 192, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 193, 1, IDEA, "Stuffcloth Idea", "" },
   { 194, 1, IDEA, "Softglass Idea", "" },
   { 195, 1, IDEA, "Albamare Key Idea", "" },
   { 196, 1, EQUIPMENT, "Beagalite Helmet", "A sturdy, well balanced helmet, excellent for protecting one's head." },
   { 197, 1, EQUIPMENT, "Beagalite Plates", "An exquisite piece of body armour, absorbs a large amount of force when struck." },
   { 198, 1, EQUIPMENT, "Beagalite Breeches", "Solid yet flexible slacks, provides great protection against incoming weapons." },
   { 199, 1, IDEA, "Wedgewood Wrist Guards Idea", "" },
   { 200, 1, IDEA, "Wedgewood Tunic Idea", "An Idea for a moss and wood-based outfit that protects against friction and basic impacts." },
   { 201, 1, IDEA, "Wedgewood Leg Guards Idea", "" },
   { 202, 1, IDEA, "Outgoer's Arm Guards Idea", "" },
   { 203, 1, IDEA, "Outgoer's Jacket Idea", "" },
   { 204, 1, IDEA, "Outgoer's Trousers Idea", "" },
   { 205, 1, IDEA, "Outgoer's Shinguards Idea", "" },
   { 206, 1, IDEA, "Staker's Arm Guards Idea", "" },
   { 207, 1, IDEA, "Staker's Jacket Idea", "" },
   { 208, 1, IDEA, "Staker's Trousers Idea", "" },
   { 209, 1, IDEA, "Staker's Shinguards Idea", "" },
   { 210, 1, IDEA, "Hunter's Vest Idea", "" },
   { 211, 1, IDEA, "Hunter's Pants Idea", "" },
   { 212, 1, IDEA, "Hunter's Boots Idea", "" },
   { 213, 1, IDEA, "Hunter's Arm Guards Idea", "" },
   { 214, 1, IDEA, "Nomad's Scarf Idea", "" },
   { 215, 1, IDEA, "Nomad Sleeves Idea", "" },
   { 216, 1, IDEA, "Nomad Pants Idea", "" },
   { 217, 1, IDEA, "Telkin Cap Idea", "" },
   { 218, 1, IDEA, "Telkin Gloves Idea", "" },
   { 219, 1, IDEA, "Telkin Boots Idea", "" },
   { 220, 1, IDEA, "Antler Mask Idea", "" },
   { 221, 1, IDEA, "Antler Braces Idea", "" },
   { 222, 1, IDEA, "Antler Shinguards Idea", "" },
   { 223, 1, IDEA, "Spitter's Hood Idea", "" },
   { 224, 1, IDEA, "Spitter's Wristguards Idea", "" },
   { 225, 1, IDEA, "Spitter's Legs Idea", "" },
   { 226, 1, IDEA, "Barrel Mask Idea", "" },
   { 227, 1, IDEA, "Barrel Gloves Idea", "" },
   { 228, 1, IDEA, "Barrel Boots Idea", "" },
   { 229, 1, IDEA, "Skull Horns Idea", "" },
   { 230, 1, IDEA, "Skull Arm Braces Idea", "" },
   { 231, 1, IDEA, "Skull Leg Braces Idea", "" },
   { 232, 1, IDEA, "Feather Adornment Idea", "" },
   { 233, 1, IDEA, "Feather Sleeves Idea", "" },
   { 234, 1, IDEA, "Feather Shinguards Idea", "" },
   { 235, 1, IDEA, "Beagalite Helmet Idea", "" },
   { 236, 1, IDEA, "Beagalite Plates Idea", "" },
   { 237, 1, IDEA, "Beagalite Breeches Idea", "" },
   { 238, 1, EQUIPMENT, "Wedgewood Wrist Guards", "Simple wooden protectors to traverse the woods more easily." },
   { 239, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 240, 999, AMMUNITION, "Sharp Arrow", "Elementary arrows to fire with bows. Deal straighforward damage." },
   { 241, 1, EQUIPMENT, "Sling Pebbles", "An infinite source of small rocks to shoot with a slingshot." },
   { 242, 999, AMMUNITION, "Broadhead Arrow", "Highly damaging arrows for body shots. Deal a great amount of damage, not good for damaging weakspots." },
   { 243, 999, AMMUNITION, "Staggering Arrow", "Penetrating arrows specialized for hitting a weakspot. Deals a good amount of damage and knocks back any opponent." },
   { 244, 100, AMMUNITION, "Explosive Arrow", "Arrow containing explosive liquid that explodes on impact." },
   { 245, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 246, 1, EQUIPMENT, "Old Pinewood Sword", "A small handmade pine-wood sword you got from the dark cavern under your home." },
   { 247, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 248, 1, EQUIPMENT, "Wedgewood Bow", "A basic bow made of pine-wood, used for dealing medium damage at medium range." },
   { 249, 1, EQUIPMENT, "Wedgewood Shield", "A pine-wood shield for protection against basic attacks." },
   { 250, 1, EQUIPMENT, "Outgoer's Sword", "A simple, one-handed sword to deal basic damage with." },
   { 251, 1, EQUIPMENT, "Outgoer's Shield", "A common wooden shield with a sturdy Lean-iron binding." },
   { 252, 1, EQUIPMENT, "Hunter's Bow", "A strong, common bow used mainly for hunting critters." },
   { 253, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 254, 1, EQUIPMENT, "Nomad Bow", "A breezy ranged weapon for quick firing under hot circumstances." },
   { 255, 1, EQUIPMENT, "Nomad Club", "A light but sturdy melee weapon used in the most arid of regions." },
   { 256, 1, EQUIPMENT, "Telkin Mace", "A heavy, trunk-like melee weapon, devised by Albamare's biggest creatures." },
   { 257, 1, EQUIPMENT, "Spitter Bow", "A bow crafted by Litters, solidified with Spystal." },
   { 258, 1, EQUIPMENT, "Beagalite Axe", "An extemely sturdy axe for highly damaging blows." },
   { 259, 1, EQUIPMENT, "Beagalite Shield", "A shield as strong as they come, with Albamare's sturdiest material at its core." },
   { 260, 1, EQUIPMENT, "Hunter's Sword", "A heavy blade to deal heavy damage with." },
   { 261, 1, SORT_OF_ITEM, "Item Name (Looks like a shield)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 262, 1, EQUIPMENT, "Antler Mace", "A heavy and sturdy weapon built by Cariblin tribes." },
   { 263, 1, EQUIPMENT, "Antler Shield", "A sturdy shield made from the Cariblin's best Lean-iron and Marrwood." },
   { 264, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 265, 1, EQUIPMENT, "Spitter Axe", "A Spystal-based axe prepared with a certain type of saliva to make its piercings stick longer." },
   { 266, 1, EQUIPMENT, "Skull Mace", "A thin but solid weapon, made of marrow and bone of deceased species." },
   { 267, 1, EQUIPMENT, "Skull Shield", "A bone-built shield, made from hippo and bird remainders for strong protection." },
   { 268, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 269, 1, EQUIPMENT, "Barrel Shield", "A wood-based shield, seemingly made to withhold heavy blows from chemical weaponry." },
   { 270, 1, EQUIPMENT, "Feather Sickle", "A razor-sharp, lightweight melee weapon resembling the claws of a certain avian species." },
   { 271, 1, EQUIPMENT, "Feather Bow", "A lightweight, sturdy bow using glossy feathers in its core." },
   { 272, 1, IDEA, "Wedgewood Bow Idea", "" },
   { 273, 1, IDEA, "Wedgewood Shield Idea", "" },
   { 274, 1, IDEA, "Outgoer's Sword Idea", "" },
   { 275, 1, IDEA, "Outgoer's Shield Idea", "" },
   { 276, 1, IDEA, "Hunter's Bow Idea", "" },
   { 277, 1, SORT_OF_ITEM, "Item Name (Looks like an Idea)", "" },
   { 278, 1, IDEA, "Nomad Bow Idea", "" },
   { 279, 1, IDEA, "Nomad Club Idea", "" },
   { 280, 1, IDEA, "Telkin Mace Idea", "" },
   { 281, 1, IDEA, "Spitter Bow Idea", "" },
   { 282, 1, IDEA, "Beagalite Axe Idea", "" },
   { 283, 1, IDEA, "Beagalite Shield Idea", "" },
   { 284, 1, IDEA, "Hunter's Sword Idea", "" },
   { 285, 1, SORT_OF_ITEM, "Item Name (Looks like an Idea 2: Electric Boogaloo)", "" },
   { 286, 1, IDEA, "Antler Mace Idea", "" },
   { 287, 1, IDEA, "Antler Shield Idea", "" },
   { 288, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 289, 1, IDEA, "Spitter Axe Idea", "" },
   { 290, 1, IDEA, "Skull Mace Idea", "" },
   { 291, 1, IDEA, "Skull Shield Idea", "" },
   { 292, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 293, 1, IDEA, "Barrel Shield Idea", "" },
   { 294, 1, IDEA, "Feather Sickle Idea", "" },
   { 295, 1, IDEA, "Feather Bow Idea", "" },
   { 296, 70, OTHER, "Amphiscus Orbs", "A rare, mysterious orb, seemingly from the civilization of an ancient species. Seems to hold special meaning." },
   { 297, 1, IDEA, "Sharp Arrow Idea", "" },
   { 298, 1, EQUIPMENT, "Wedgewood Sword", "An pine-wood sword that deals little damage but cuts deep." },
   { 299, 1, IDEA, "Wedgewood Sword Idea", "" },
   { 300, 1, IMPORTANT, "Pouch Upgrade 1", "A slightly larger pouch to carry your items in." },
   { 301, 1, IMPORTANT, "Pouch Upgrade 2", "A large pouch to carry many items." },
   { 302, 10, TOKEN, "Gatherer Token", "An order and badge of a gatherer, tasked by the leader of a village to check the nearby surroundings and collect food and materials." },
   { 303, 10, TOKEN, "Trader Token", "An order and badge of a trader, tasked by the leader of a village to trade with a neighbouring village." },
   { 304, 10, TOKEN, "Guard Token", "An order and badge of a village guard, tasked by the leader to protect a territory at all cost." },
   { 305, 10, TOKEN, "Raider Token", "An order and badge of a raider, tasked by the leader of a village to invade a settlement." },
   { 306, 1, EQUIPMENT, "Amam's Slingshot", "A simple, handmade slingshot you got from your brother." },
   { 307, 1, SORT_OF_ITEM, "Item Name (Looks like a slingshot)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 308, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 309, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 310, 10, SIMULATION, "Preservation Knowledge", "A scroll containing methods of food and material preservation, boosting the village banks. Seems to let the ancient material Droolica reappear in villages. Lasts 5 minutes." },
   { 311, 10, SIMULATION, "False Commision", "A fabricated order for extra food and armor, causing a village to increase its expenses and overproduce. This will last for 5 minutes." },
   { 312, 10, SIMULATION, "Incoming Storm Rumor", "A rumor about a large incoming storm, for which villagers will make haste. The leader will order more gather and trade parties for 5 minutes." },
   { 313, 10, SIMULATION, "Incoming Heat Wave Rumor", "A rumor about an incoming heat wave, making villagers slow down. The village will send out fewer trading and gathering parties for 5 minutes." },
   { 314, 10, SIMULATION, "Tranquility Rumor", "A message spreading a vibe of peace and tranquility, making the village rapidly increase its population. The leader of the village will order its subjects to be peaceful towards all other factions for 5 minutes." },
   { 315, 10, SIMULATION, "Cataclysm Rumor", "A rumor of an incoming cataclysm, making a village enter survival mode, starting to decrease its population. The leader of the village will order its subjects to be aggressive towards all factions for 5 minutes." },
   { 316, 10, SIMULATION, "Peace Treaty", "A document containing a treaty of fruitful cooperation. The involved parties will greatly improve their affinity relations." },
   { 317, 10, SIMULATION, "Written Insult", "An insulting message containing a variety of threats. Involved parties will greatly worsen their affinity relations." },
   { 318, 10, SIMULATION, "Efficiency Plan", "An elaborate plan about more efficient material usage. Villagers possessing this will produce stronger armor and will upgrade much quicker. The effect lingers for 5 minutes." },
   { 319, 10, SIMULATION, "Faulty Designs", "A false plan that introduces bad designs for armor and structure, causing villages to need a lot more resources to upgrade and build low-quality armor. The effect lingers for 5 minutes." },
   { 320, 1, SORT_OF_ITEM, "Item Name (Looks like a simulation item with down arrow)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 321, 5, SIMULATION, "Rotten Meat Platter", "Large platter of meat that was seasoned well to cover its rot. Village inhabitants that eat it will get sick which will weaken them in combat. The health effect lingers for 5 minutes." },
   { 322, 5, SIMULATION, "Poisoned Meat Platter", "Large platter of meat that was treated with a poisonous rub. Village inhabitants that eat it will get a fever which will weaken them strongly in combat. The effect of the poison lingers for 5 minutes." },
   { 323, 3, SIMULATION, "Highly Toxic Meat Platter", "Large platter of meat that was marinated in toxic juice. Village inhabitants that eat it will get extremely sick causing them to be very weak in combat. The effect lingers for 5 minutes." },
   { 324, 0, NO_ITEM, "", "" },
   { 325, 0, NO_ITEM, "", "" },
   { 326, 0, NO_ITEM, "", "" },
   { 327, 0, NO_ITEM, "", "" },
   { 328, 0, NO_ITEM, "", "" },
   { 329, 0, NO_ITEM, "", "" },
   { 330, 0, NO_ITEM, "", "" },
   { 331, 0, NO_ITEM, "", "" },
   { 332, 0, NO_ITEM, "", "" },
   { 333, 0, NO_ITEM, "", "" },
   { 334, 0, NO_ITEM, "", "" },
   { 335, 0, NO_ITEM, "", "" },
   { 336, 0, NO_ITEM, "", "" },
   { 337, 0, NO_ITEM, "", "" },
   { 338, 3, TRAP, "Explosive Keg", "A highly unstable brew that reacts on impact. Any impact will cause a large explosion that can pierce any armor and remove fragile structures." },
   { 339, 6, TRAP, "Explosive Tripwire", "A sneaky contraption that explodes when steppd on." },
   { 340, 6, TRAP, "Sparkler Trap", "A contraption that scares creatures when activated. When triggered outside of villages, creatures panic and drop their current belongings." },
   { 341, 5, RAID_FLARE, "Cariblin Raid Flare", "A contraption that orders a Cariblin raid group. Must be deployed next to a village that does not belong to Cariblin." },
   { 342, 5, RAID_FLARE, "Fexel Raid Flare", "A contraption that orders a Fexel raid group. Must be deployed next to a village that does not belong to Fexels." },
   { 343, 5, RAID_FLARE, "Gobbledew Raid Flare", "A contraption that orders a Gobbledew raid group. Must be deployed next to a village that does not belong to Gobbledew." },
   { 344, 5, RAID_FLARE, "Krocker Raid Flare", "A contraption that orders a Krocker raid group. Must be deployed next to a village that does not belong to Krockers." },
   { 345, 5, RAID_FLARE, "Litter Raid Flare", "A contraption that orders a Litter raid group. Must be deployed next to a village that does not belong to Litters." },
   { 346, 1, IDEA, "Sparkler Trap Idea", "" },
   { 347, 1, IDEA, "Explosive Keg Idea", "" },
   { 348, 1, IDEA, "Explosive Keg Idea", "" },
   { 349, 1, IDEA, "Preservation Knowledge Idea", "" },
   { 350, 1, IDEA, "False Commision Idea", "" },
   { 351, 1, IDEA, "Incoming Storm Rumor Idea", "" },
   { 352, 1, IDEA, "Incoming Heat Wave Rumor Idea", "" },
   { 353, 1, IDEA, "Tranquility Rumor Idea", "" },
   { 354, 1, IDEA, "Cataclysm Rumor Idea", "" },
   { 355, 1, IDEA, "Peace Treaty Idea", "" },
   { 356, 1, IDEA, "Written Insult Idea", "" },
   { 357, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 358, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 359, 1, SORT_OF_ITEM, "Item Name (Looks like a treaty/rumor idea)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 360, 1, IDEA, "Rotten Meat Platter Idea", "" },
   { 361, 1, IDEA, "Poisoned Meat Platter Idea", "" },
   { 362, 1, IDEA, "Highly Toxic Meat Platter Idea", "" },
   { 363, 1, IDEA, "Efficiency Plan Idea", "" },
   { 364, 1, IDEA, "Faulty Designs Idea", "" },
   { 365, 1, SORT_OF_ITEM, "Item Name (Looks like a sword)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 366, 1, SORT_OF_ITEM, "Item Name (Looks like a sword idea)", "Stone tablets, dating from an older Human civilisation. Highly valued by merchants." },
   { 367, 1, IDEA, "Broadhead Arrow Idea", "" },
   { 368, 1, IDEA, "Staggering Arrow Idea", "" },
   { 369, 10, MATERIAL, "Droolica", "A byproduct of grinding stone with Litter spit. Every grain is extremely hard and has the ability to draw out moisture, making it ideal for preserving food and other materials." },
   { 370, 1, IMPORTANT, "King Drool's Spystal Fragment", "A rare breed of Spystal engulfed in Litter spit. It is radiating light, and is mesmerizing for whoever gazes at it." },
   { 371, 1, IMPORTANT, "Fermented Meageryam", "A Meageryam that a Litter chief has been sitting on for a week. It has nutritional value, but smells heavy and musky. Fully depletes your energy instantly." },
   { 372, 1, IMPORTANT, "Mechanical Lever", "Seemingly simple contraption that played a big role for the technological advancement of Albamare. Similar mechanisms have become a part of the daily life on the island." },
   { 373, 1, IMPORTANT, "Bloodberries", "Mysterious pieces of food that reek of Lean-iron. Has a moldy feel and a slight pulsation. Not very apetizing..." },
   { 374, 1, IMPORTANT, "Dark Beast Bait", "Incredibly pungent bait. The size of it suggest that whatever it is to be hunted is extremely large and dangerous." },
   { 375, 1, IMPORTANT, "Dark Beast's Eye", "The eye of a poweful, unique Bleeker. It's pulsating and radiating energy." },
   { 376, 1, IMPORTANT, "Garment Crassbone Pin", "Part of the Krocker chief garment. The pin seems carved and straightened intentionally." },
   { 377, 1, IMPORTANT, "Beagalite Antenna Cap", "A crudely engineered part, meant to block the conducting capabilities of Mohlenite." },
   { 378, 1, IMPORTANT, "Pearl Necklace Gift", "An extravagant piece of jewelery. The excessive use of Slick Pearl, one of the most expensive materials on Albamare, is meant to impress royal blood." },
   { 379, 1, IDEA, "Pearl Necklace Gift Idea", "" },
   { 380, 1, IMPORTANT, "Sibblesunc Alphabet", "Scroll containing symbols that form the Sibblesunc interpretation on the common Albamare language." },
   { 381, 1, IDEA, "Fexel Raid Flare Idea", "" },
   { 382, 1, IMPORTANT, "Litter Council Invitation", "A call for unity between all species on the island of Albamare. This one is addressed to the Litters." },
   { 383, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 384, 1, IMPORTANT, "Tiny Hidden Key", "A small key with unknown ability." },
   { 385, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 386, 1, IDEA, "Explosive Arrow Idea", "" },
   { 387, 1, EQUIPMENT, "Wedgewood Pants", "A slightly more solid pair of pants, based on Gravel Moss and Stiffrope." },
   { 388, 1, IDEA, "Wedgewood Pants Idea", "Idea for a slightly more solid pair of pants, based on Gravel Moss and Stiffrope." },
   { 389, 1, IDEA, "Litter Raid Flare Idea", "" },
   { 390, 1, IDEA, "Krocker Raid Flare Idea", "" },
   { 391, 1, IDEA, "Cariblin Raid Flare Idea", "" },
   { 392, 1, IDEA, "Gobbledew Raid Flare Idea", "" },
   { 393, 1, EQUIPMENT, "Beagalite Gauntlets", "Segmented gauntlets, protecting your fingers from dismemberment." },
   { 394, 1, EQUIPMENT, "Beagalite Sabatons", "Uncomfortable sabatons, protecting your toes from dismemberment as well as keeping your feet warm. Very warm." },
   { 395, 1, EQUIPMENT, "Barrel Bow", "A sturdy bow made of the Fexel's lenient wood, solidified with their alchemy-proof metals." },
   { 396, 1, IDEA, "Barrel Bow Idea", "" },
   { 397, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 398, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 399, 50, MIXTURE, "Krocker Brew", "A specific mixture created by the Krockers that makes you less vulnerable and greatly increased shield stamina." },
   { 400, 50, MIXTURE, "Cariblin Potion", "A mixture created by the Cariblins that focuses on melee damage by greatly increasing it, but also makes you more vulnerable." },
   { 401, 50, MIXTURE, "Litter Broth", "A rather disgusting brew from the Litters that makes you less vulnerable and greatly increases your ranged attack speed." },
   { 402, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 403, 50, MIXTURE, "Gobbledew Melange", "A beautiful liquid by the Gobbledew that makes you a small movement bonus and greatly increases melee attack speed." },
   { 404, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 405, 1, IDEA, "Alternate Stuffcloth Idea", "" },
   { 406, 1, IMPORTANT, "Scavenger's Fragment", "A mysterious fragment placed by a traveler, whom's notes you found in the Pollen Pass. Seems to be part of a set." },
   { 407, 50, MIXTURE, "Fexel Blend", "A smartly conjured up mixture by the Fexels that focuses on an increased ranged attack damage, but also makes you a bit more vulnerable." },
   { 408, 1000, OTHER, "Travel Kit", "A craftable kit that can transfer you to the Tamba house or to the village of an allied species when used." },
   { 409, 1, IDEA, "Travel Kit Idea", "" },
   { 410, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." },
   { 411, 0, GAME_BREAKING_ITEM, "Game Breaker", "Causes save file to not load." }
};

#endif // !GAMEINFO_H
