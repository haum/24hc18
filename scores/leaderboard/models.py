from django.db import models, IntegrityError
from django.utils.crypto import get_random_string

import itertools
from random import randint
from math import floor


class Team(models.Model):
    """ Holds a team
    name -- name of the team
    points -- number of points
    repo_url -- url of the git repo
    place -- where the team sits
    """

    name = models.CharField(max_length=100, unique=True)
    points = models.IntegerField(default=0)
    repo_url = models.CharField(max_length=400, unique=True)
    place = models.CharField(max_length=200, blank=True, default="")
    valid_repo = models.BooleanField(default=False)
    valid_buildscript = models.BooleanField(default=False)
    valid_startscript = models.BooleanField(default=False)

    def __str__(self):
        return self.name

    def update_score(self):
        self.points = sum([_.points_earned for _ in self.participation_set.filter(match__is_finished=True)])
        self.save()


class BackendAuth(models.Model):
    """ Registers backend token
    name -- planned usage for the token
    token -- token itself (32 to 40 chars string)
    active -- is the token valid now
    note -- additional info
    """
    name = models.CharField(max_length=100, unique=True)
    token = models.CharField(max_length=200, blank=True, unique=True)
    active = models.BooleanField(default=False)
    note = models.CharField(max_length=200, blank=True, null=True)

    def __str__(self):
        return f"[Auth] {self.name}"

    def generate_token(self):
        existing_tokens = BackendAuth.objects.values_list('token', flat=True)
        is_unique = False
        while not is_unique:
            token = get_random_string(length=randint(32,40))
            is_unique = token not in existing_tokens
        self.token = token
        self.save()

    def save(self):
        if not self.token:
            self.generate_token()
        else:
            super().save()


class BackendAction(models.Model):
    """ Keeps track of the actions performed using tokens
    name -- action's identifier
    desccription -- lengthy description of what happened
    date -- automatic datetime field
    auth -- token used to perform the action
    """

    name = models.CharField(max_length=100)
    description = models.CharField(max_length=500)
    date = models.DateTimeField(auto_now_add=True)
    auth = models.ForeignKey('BackendAuth', on_delete=models.CASCADE)


class Match(models.Model):
    """ Stores a match between n-teams
    teams -- teams engaged (the through-model stores the points)
    is_finished -- match's status
    group -- corresponding group
    """
    teams = models.ManyToManyField(
        Team,
        through='Participation',
        through_fields=('match', 'team')
    )
    date = models.DateTimeField(auto_now_add=True)
    is_finished = models.BooleanField(default=False)
    group = models.ForeignKey('MatchGroup', on_delete=models.SET_NULL, null=True, blank=True)

    def check_is_finished(self):
        for p in self.participation_set.all():
            if p.points_earned is None:
                self.is_finished = False
                break
        self.is_finished = True
        return self.is_finished

    def __str__(self):
        return f'Match #{self.id} - {self.date} ({self.teams.count()} teams)'


class Participation(models.Model):
    """ Holds the number of points won by a team in a match """
    points_earned = models.IntegerField(null=True)
    team = models.ForeignKey('Team', on_delete=models.CASCADE)
    match = models.ForeignKey('Match', on_delete=models.CASCADE)

    def __str__(self):
        return f'Participation of team {self.team} in match #{self.match.id}'


class MatchGroup(models.Model):
    """ Groups matches together to gather all corresponding to the same game phase
    name -- group's name
    """
    name = models.CharField(max_length=100, help_text="This is pupblic, choose wisely.")
    teams = models.ManyToManyField(Team, help_text="Select the teams that will participate")
    scenario = models.CharField(max_length=200, help_text="scenario_file.cfg with root in engine/scenarii")
    number_playing_teams = models.IntegerField(default=1, help_text="The number of teams this scenario is made for")
    num_pools = models.IntegerField(default=1, help_text="Number of pools to use when matching teams")
    matches_generated = models.BooleanField(default=False)

    def __str__(self):
        return self.name

    def generate_matches(self):
        pool_size = floor(self.teams.count()/self.num_pools)
        ordered_teams = self.teams.order_by('-points')
        for i_p in range(self.num_pools):
            begin = pool_size*i_p
            end = begin+pool_size if i_p != self.num_pools-1 else None
            teams_in_pool = ordered_teams[begin:end]

            for team_set in itertools.combinations(teams_in_pool, self.number_playing_teams):
                m = Match.objects.create(group=self)
                for t in team_set:
                    Participation.objects.create(match=m, team=t)
        self.matches_generated = True
        self.save()

    def finish_matches(self):
        """End all matches linked to this group and update teams scores"""
        for m in self.match_set.filter(is_finished=False):
            m.participation_set.all().update(points_earned=0)
        self.match_set.filter(is_finished=False).update(is_finished=True)
        for t in self.teams.all():
            t.update_score()
