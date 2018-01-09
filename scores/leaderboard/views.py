from django.shortcuts import render, get_object_or_404
from django.urls import reverse
from django.http import HttpResponseRedirect, HttpResponseForbidden, Http404
from django.core.exceptions import PermissionDenied
from django.db.models import Q

from .models import Team, BackendAuth, BackendAction, Match, Participation, MatchGroup
from .forms import TeamRegisterForm

def check_token(f):

    def w(request, token, *args, **kwargs):
        nb_corresponding_auth =  BackendAuth.objects.filter(token=token, active=True).count()
        if nb_corresponding_auth == 0:
            raise PermissionDenied('No such token has been registered')
        elif nb_corresponding_auth > 1:
            corresponding_auth =  BackendAuth.objects.filter(token=token, active=True)
            for a in corresponding_auth:
                a.active = False
                a.save()
            raise PermissionDenied('The token was found to be corrupted. All impacted credentials have been desactivated.')
        return f(request, *args, **kwargs)

    return w

def api_404(request, error, description):
    """ Returns a API-specific rendered page w/ status 404
    request  -- to be passed to render()
    error -- error name
    description -- long description
    """
    return render(
        request,
        'backend/404.html',
        {
            'error': error,
            'description': description
        })


def api_ok(request, msg=''):
    return render(request, 'backend/api_ok.html', {'msg': msg}, status=404)



def home(request):
    teams = Team.objects.all().order_by('-points')
    matchs = Match.objects.all().order_by('-date')
    return render(request, 'home.html', {'teams': teams, 'matchs': matchs})


def register_team(request):
    """
    Process/propose the team registration form
    """
    if request.method == 'POST':
        form = TeamRegisterForm(request.POST)

        if form.is_valid():
            name = form.cleaned_data['name']
            repo_url = form.cleaned_data['repo_url']
            # TODO : check URL validity

            team = Team(name=name, repo_url=repo_url)
            team.save()

            return HttpResponseRedirect(reverse('thanks'))

    else:
        form = TeamRegisterForm()

    return render(request, 'register_form.html', {'form': form})


def team_detail(request, team_id):
    team = get_object_or_404(Team, pk=team_id)
    return render(request, 'team_detail.html', {'t': team})


def match_detail(request, match_id):
    match = get_object_or_404(Match, pk=match_id)
    teams = match.teams.all()
    return render(request, 'match_detail.html', {'m': match, 'teams': teams})


@check_token
def generate_repofile(request):
    """
    Generate and outputs a file containing the location of
    all repos and callback url for the teams registered as competing
    """

    teams = Team.objects.filter(valid_repo=True, valid_buildscript=True, valid_startscript=True)
    return render(request, 'backend/generate_repofile.html', {'teams': teams})


@check_token
def generate_runfile(request, group_id):
    """
    Generate a file containing start instructions for all unfinished matches in specified
    groups.
    """

    group = get_object_or_404(MatchGroup, pk=group_id)
    matches = group.match_set.filter(is_finished=False)

    return render(request, 'backend/generate_runfile.html', {'matches': matches})


@check_token
def register_match(request, group, teams_str):
    """
    Register a new match between the teams in 'teams' and associate it to group 'group'
    """

    if group == "none":
        match_group = None
    else:
        match_group = get_object_or_404(MatchGroup, pk=group)

    # teams are passed as a colon-separated list of ids
    teams = list(map(int, teams_str.split(':')))

    query = Q(pk=teams[0])
    for t in teams[1:]: query |= Q(pk=t)

    teams_qset = Team.objects.filter(query)
    if len(teams) != len(teams_qset):
        return api_404('Object Not Found', 'Some of the teams could not be found')

    match = Match.objects.create(group=match_group)
    for t in teams_qset:
        Participation.objects.create(match=match, team=t)

    return api_ok(request, str(match.id))


@check_token
def register_score(request, match_id, team_id, score):
    """ Registers the score in a given match for a given team """

    try:
        match = Match.objects.get(pk=match_id)
    except Match.DoesNotExist:
        return api_404('Object Not Found', 'The referenced Match object could not be found.')

    try:
        p = match.participation_set.get(team=team_id)
    except Participation.DoesNotExist:
        return api_404('Object Not Found', 'The referenced Team object could not be found.')

    p.points_earned = score
    p.save()

    match.check_is_finished()
    match.save()

    p.team.update_score()
    p.team.save()

    return api_ok(request, 'Match updated')
