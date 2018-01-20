from django.contrib import admin

from .models import \
    Team,\
    BackendAuth,\
    BackendAction,\
    Match,\
    MatchGroup,\
    Participation


@admin.register(Team)
class TeamAdmin(admin.ModelAdmin):

    list_display = (
        'name', 'id', 'points', 'repo_url', 'place',
        'valid_repo', 'valid_buildscript', 'valid_startscript'
    )
    actions = [
        'validate_repo',
        'validate_buildscript',
        'validate_startscript',
        'validate_buildstart',
        'invalidate_buildstart'
    ]

    def validate_buildscript(self, request, queryset):
        queryset.update(valid_buildscript=True)
    validate_buildscript.short_description = "Validate build.sh"

    def validate_startscript(self, request, queryset):
        queryset.update(valid_startscript=True)
    validate_startscript.short_description = "Validate start.sh"

    def validate_repo(self, request, queryset):
        queryset.update(valid_repo=True)
    validate_repo.short_description = "Validate git repo"

    def validate_buildstart(self, request, queryset):
        queryset.update(valid_buildscript=True)
        queryset.update(valid_startscript=True)
    validate_buildstart.short_description = "Validate both start.sh & build.sh"

    def invalidate_buildstart(self, request, queryset):
        queryset.update(valid_buildscript=True)
        queryset.update(valid_startscript=True)
    invalidate_buildstart.short_description = "INvalidate both start.sh & build.sh"

@admin.register(BackendAuth)
class BackendAuthAdmin(admin.ModelAdmin):

    list_display = ('name', 'token', 'active', 'note')
    list_filter = ('active',)
    actions = ('regenerate_token',)

    def regenerate_token(self, request, queryset):

        for auth in queryset:
            auth.generate_token()

    regenerate_token.short_description = "Regenerate a token for selected Auth"


@admin.register(BackendAction)
class BackendActionAdmin(admin.ModelAdmin):

    list_display = ('date', 'auth', 'name', 'description')
    list_filter = ('auth',)


@admin.register(Match)
class MatchAdmin(admin.ModelAdmin):

    list_display = ('id', 'group', 'show_scenario', 'list_teams', 'is_finished')
    list_filter = ('group', 'is_finished', 'teams')

    def list_teams(self, obj):
        return ', '.join([_.name for _ in obj.teams.all()])

    def show_scenario(self, obj):
        return obj.group.scenario


@admin.register(Participation)
class ParticipationAdmin(admin.ModelAdmin):

    list_display = ('id', 'match', 'team', 'view_points')
    list_filter = ('team', 'points_earned')

    def view_points(self, obj):
        return obj.points_earned
    view_points.empty_value_display = '0'


@admin.register(MatchGroup)
class MatchGroupAdmin(admin.ModelAdmin):

    fieldsets= (
        (None, {
            'fields': ('name',),
        }),
        ('Scenario', {
            'fields': ('scenario', 'number_playing_teams',),
        }),
        ('Teams', {
            'fields': ('num_pools', 'teams'),
        }),
    )
    list_display = ('name', 'id', 'scenario', 'number_playing_teams', 'num_pools', 'list_teams', 'matches_generated', 'is_closed')
    list_filter = ('matches_generated', 'num_pools', 'number_playing_teams')
    actions = ['mark_finished', 'generate_matches']

    def list_teams(self, obj):
        qset = obj.teams.all()
        total_teams = Team.objects.all().count()

        if qset.count() == total_teams:
            return "*"
        elif qset.count() > 5:
            return f"{qset.count()} teams"
        else:
            return ', '.join([_.name for _ in qset])
    list_teams.short_description = 'Teams'

    def is_closed(self, obj):
        """true if all matches are finished"""
        return obj.match_set.filter(is_finished=False).count() == 0
    is_closed.boolean = True

    def mark_finished(self, request, queryset):
        for m in queryset:
            m.finish_matches()

    def generate_matches(self, request, queryset):
        for m in queryset:
            m.generate_matches()

    generate_matches.short_description = 'Generate matches for the selected groups'
    mark_finished.short_description = 'Mark related matches as finished'
