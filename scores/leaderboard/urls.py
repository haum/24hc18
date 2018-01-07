from django.urls import path
from django.views.generic import TemplateView

import leaderboard.views as views


urlpatterns = [
    path('', views.home, name='home'),
    path('register', views.register_team, name='register_team'),
    path('register/thanks', TemplateView.as_view(template_name='thanks.html'), name='thanks'),
    path('team/<int:team_id>', views.team_detail, name='team_detail'),
    path('be/t:<token>/runfile', views.generate_runfile, name='generate_runfile'),
    path('be/t:<token>/register_match/<group>/<teams_str>', views.register_match, name='register_match'),
    path('be/t:<token>/register_score/<int:match_id>/<int:team_id>/<int:score>', views.register_score, name='register_score'),
]

