class PM_UI_Option_Playerlist extends PM_UI_Category
{
    void PM_UI_Option_Playerlist(Widget parent, int index)
    {
        m_icon = "partyme/gui/images/options/icons/player.tga";
        m_w_parent = parent;
        Init(index);
    }

    override void GetWidgets()
    {
        super.GetWidgets();

        m_menu = new PM_UI_Menu_Playerlist(m_w_parent);
    }

    override void ConfigureWidget()
    {
        super.ConfigureWidget();
        m_txt_optionName.SetText("Players");
    }
};


class PM_UI_Menu_Playerlist extends PM_UI_Menu
{
    protected ScrollWidget                                  m_scroll_playerList;
    protected ref array<ref PM_UI_playerlist_PlayerWidget>  m_players;

    void PM_UI_Menu_Playerlist(Widget parent)
    {
        m_layoutPath = "partyme/gui/layouts/submenus/playerlist/options/option.layout";
        m_w_parent = parent;
        m_players = new array<ref PM_UI_playerlist_PlayerWidget>;
        Init();
        InitPlayerList();
        PM_GetEvents().AddEvent("PlayerJoinServer", this);
    }

    void InitPlayerList()
    {
        string playerId;

        if (PM_GetPlayerId(playerId))
        {
            GetRPCManager().SendRPC("PartyMe", "GetPlayerList", new Param1<string>(playerId));
        }
    }

    override void GetWidgets()
    {
        super.GetWidgets();
        m_scroll_playerList = ScrollWidget.Cast(m_w_root.FindAnyWidget("playerList"));
    }

    //--------------------------------------------------------------------------
    int FindPlayer(string playerId)
    {
        for (int index = 0; index < m_players.Count(); index++)
        {
            ref PM_UI_playerlist_PlayerWidget playerWidget = m_players.Get(index);
            if (playerWidget && playerWidget.GetPlayerId() == playerId)
            {
                return index;
            }
        }
        return -1;
    }

    void AddPlayer(string playerId, string playerName)
    {
        if (FindPlayer(playerId) == -1)
        {
            auto playerWidget = new PM_UI_playerlist_PlayerWidget(m_scroll_playerList, playerId, playerName);
            int pos = m_players.Count();
            m_players.Insert(playerWidget);
            playerWidget.SetPosition(pos);
        }
    }

     void RemovePlayer(string playerId)
    {
        int indexPlayer = FindPlayer(playerId);

        if (indexPlayer != -1)
        {
            m_players.Remove(indexPlayer);
            UpdateWidgetsPosition();
        }
    }

    void AddMissingPlayers(array<Param2<string, string>> players)
    {
        foreach (auto player : players)
        {
            AddPlayer(player.param1, player.param2);
        }
    }

    void UpdateWidgetsPosition()
    {
        for (int indexWidget = 0; indexWidget < m_players.Count(); indexWidget++)
        {
            // playerWidget.
        }
    }

    //-------------------------------------------------------------------------- Events
    void OnPlayerJoinServer(ref PM_Event_Params eventParams)
    {
        auto playerWidget = new PM_UI_playerlist_PlayerWidget(m_scroll_playerList, eventParams.playerIdFrom, eventParams.name);

        if (m_players)
        {
            playerWidget.SetPosition(m_players.Count());
            m_players.Insert(playerWidget);
        }
    }
};

class PM_UI_playerlist_PlayerWidget
{
    private static const int INVITE_DELAY                   = 2000;
    private static const string DEFAULT_LAYOUT              = "partyme/gui/layouts/submenus/playerlist/options/widgets/player.layout";
    private static const string ICON_INVITE                 = "partyme/gui/images/options/icons/invite.tga";

    private Widget                                          m_w_root;
    private TextWidget                                      m_txt_playerName;
    private ButtonWidget                                    m_btn_invite;
    private ImageWidget                                     m_img_invite;

    private string                                          m_playerId;
    private string                                          m_playerName;
    private bool                                            m_canInvite;

    void PM_UI_playerlist_PlayerWidget(Widget parent, string playerId, string playerName)
    {
        m_playerId = playerId;
        m_playerName = playerName;
        m_canInvite = true;
        m_w_root = GetGame().GetWorkspace().CreateWidgets(DEFAULT_LAYOUT, parent);
        GetWidgets();
        ConfigWidgets();
    }

    private void GetWidgets()
    {
        m_btn_invite = ButtonWidget.Cast(m_w_root.FindAnyWidget("invite"));
        m_txt_playerName = TextWidget.Cast(m_w_root.FindAnyWidget("playerName"));
        m_img_invite = ImageWidget.Cast(m_w_root.FindAnyWidget("logo"));

    }

    private void ConfigWidgets()
    {
        m_txt_playerName.SetText(m_playerName);
        InitIcons();
    }

    private void InitIcons()
    {
        m_img_invite.LoadImageFile(0, ICON_INVITE);
    }

    void SetPosition(int index)
    {
        m_w_root.SetPos(50, (40 + 65 * index));
    }

    private void EnableInviteButton()
    {
        m_btn_invite.Enable(true);
        m_img_invite.Show(true);
    }

    private void DisableInviteButton()
    {
        m_btn_invite.Enable(false);
        m_img_invite.Show(false);
    }

    //--------------------------------------------------------------------------
    string GetPlayerId()
    {
        return m_playerId;
    }

    string GetPlayerName()
    {
        return m_playerName;
    }

    //--------------------------------------------------------------------------
    bool OnClick(Widget w, int x, int y, int button)
    {
        if (button == MouseState.LEFT)
        {
            if (w == m_btn_invite && m_canInvite && PM_GetGroup().CanInvite())
            {
                Invite();
                return true;
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------
    private void Invite()
    {
        DisableInviteButton();
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(EnableInviteButton, INVITE_DELAY);
        // SEND RPC
    }
};