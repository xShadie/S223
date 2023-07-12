/*
  .oooooo.   oooooo   oooo ooooo      ooo   .oooo.
 d8P'  `Y8b   `888.   .8'  `888b.     `8' .dP""Y88b
888            `888. .8'    8 `88b.    8        ]8P'
888             `888.8'     8   `88b.  8      <88b.
888              `888'      8     `88b.8       `88b.
`88b    ooo       888       8       `888  o.   .88P
 `Y8bood8P'      o888o     o8o        `8  `8bd88P'

Version:
	Release 1.0.0

Contacts:
	Metin2Downloads: https://www.metin2downloads.to/cms/user/11018-cyn3/
	Discord: Franky#6315

Support:
	Discord: https://discord.gg/sura-head

Special thanks to:
	- Rainer <- Design & Animations
	- SamaLamaDingDong <- Code review (Server - Source)
	- <> <- Mental support
*/

#include "../../common/singleton.h"
#include "../../common/service.h"
#include <unordered_map>
#include "constants.h"
#ifdef ENABLE_ITEMSHOP
class CItemshopManager : public singleton<CItemshopManager>
{
public:
	CItemshopManager();
	~CItemshopManager();

	void InitializeItems(TItemshopItemTable* table, WORD size);
	void InitializeCategories(TItemshopCategoryTable* table, WORD size);

	void RefreshSingleItem(TItemshopItemTable* item);
	void RemoveSingleItem(TItemshopItemTable* item);
	void AddSingleItem(TItemshopItemTable* item);

	bool IsValidHash(const char* hash);
	bool HasEnoughCoins(DWORD accID, unsigned long long ullItemPrice);
	bool IsValidPurchase(TItemshopItemTable itemInfo, WORD wCount, unsigned long long &ullPrice);
	bool CanBuyItem(LPCHARACTER ch, const char* hash, WORD wCount);
	bool BuyItem(LPCHARACTER ch, const char* hash, WORD wCount);

	const std::unordered_map <BYTE, TItemshopCategoryInfo> GetItemshopCategories();
	const std::unordered_map <BYTE, std::vector<TItemshopItemTable>> GetItemshopItems();

	std::unordered_map <BYTE, TItemshopCategoryInfo> m_ItemshopCategories;
	std::unordered_map <BYTE, std::vector<TItemshopItemTable>> m_ItemshopItems;
};
#endif
