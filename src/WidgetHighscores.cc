#include "WidgetHighscores.hh"
#include "HighscoreEntry.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemHighscores.hh"


WidgetHighscores::WidgetHighscores(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size),
    _hilited(nullptr),
    _last_refresh(0)
{
  Refresh();
}


void WidgetHighscores::Tick(double deltatime)
{
  Widget::Tick(deltatime);
  
  auto cursize = static_cast<unsigned int>(Highscores->Get().size());
  if(cursize != _last_refresh)
    {
      _last_refresh = cursize;
      Refresh();
    }
}


void WidgetHighscores::Refresh()
{
  DestroyChildren();
  
  int y = 0;

  auto title = new Widget(this, glm::ivec2(0, y), glm::ivec2(200, 30));
  title->SetText("Highscores:");
  title->SetTextFont(AssetLoader->LoadFont(14));
  title->SetTextPaddingCentered(false, true);
  title->SetTextColor(glm::vec3(0, 1, 1));
  y += title->GetSize().y;
  
  auto entries = Highscores->Get();
  for(unsigned int i = 0; i < 5 && i < entries.size(); i++)
    {
      auto w = new Widget(this, glm::ivec2(10, y), glm::ivec2(200, 25));
      w->SetText(std::to_string(i + 1) + ".  " + std::to_string(entries[i]->GetScore()));
      if(entries[i] == _hilited)
        {
          w->SetTextFont(AssetLoader->LoadFont(13));
          w->SetTextColor(glm::vec3(0, 1, 1));
        }
      else
        {
          w->SetTextFont(AssetLoader->LoadFont(12));
          w->SetTextColor(glm::vec3(0, 1, 1) * 0.5f);
        }
      w->SetTextPaddingCentered(false, true);

      y += w->GetSize().y;
    }
}
