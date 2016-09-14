#pragma once

VENUS_BEG

class V2D_API ITreeViewItem : public IViewItem
{
public:

};

/**
* 树状控件的项目类型。
*/

class TreeItemT : public ObjectT<ITreeViewItem>
{
public:
	typedef vector<object_ptr<TreeItemT>> TreeItemCollection;

	TreeItemT()
		: Parent(nullptr), Image(nullptr),
		ContentWidth(0), ContentHeight(0),
		ItemWidth(0), ItemHeight(0),
		Width(0), Height(0),
		UserData(0), Flags(0)
	{
	}

	TreeItemT(const char_16 * szText, IImage * pImage = nullptr,
		TreeItemT * pParent = nullptr, int_x iUserData = 0)
		: Parent(pParent), Image(object_ref(pImage)),
		ContentWidth(0), ContentHeight(0),
		ItemWidth(0), ItemHeight(0),
		Width(0), Height(0),
		Text(szText),
		UserData(iUserData), Flags(0)
	{
	}

	~TreeItemT()
	{
		Items.clear();
	}

	int_x GetDepth() const
	{
		return Parent ? Parent->GetDepth() + 1 : 0;
	}

	void SetUserData(int_x iUserData) { UserData = iUserData; }
	int_x GetUserData() const { return UserData; }

	bool CanExpand() const { return Items.is_valid() || ForceExpandable; }
	textw Text;
	TreeItemT * Parent;
	object_ptr<IImage> Image;

	int_x ContentWidth;
	int_x ContentHeight;

	int_x ItemWidth;
	int_x ItemHeight;

	int_x Width;
	int_x Height;

	TreeItemCollection Items;
	int_x UserData;
	union
	{
		int_x Flags;
		struct
		{
			bool ValidContent : 1;
			bool Valid : 1;

			bool Expanded : 1;
			bool Selected : 1;
			bool HighLighted : 1;
			bool ForceExpandable : 1;
		};
	};
};

VENUS_END
