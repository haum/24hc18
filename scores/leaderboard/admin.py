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
        'name', 'points', 'repo_url', 'place',
        'valid_repo', 'valid_buildscript', 'valid_startscript'
    )


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

    list_display = ('id', 'group', 'list_teams', 'is_finished')
    list_filter = ('group', 'is_finished', 'teams')

    def list_teams(self, obj):
        return ', '.join([_.name for _ in obj.teams.all()])


@admin.register(Participation)
class ParticipationAdmin(admin.ModelAdmin):

    list_display = ('id', 'match', 'team', 'view_points')
    list_filter = ('team', 'points_earned')

    def view_points(self, obj):
        return obj.points_earned
    view_points.empty_value_display = '0'


@admin.register(MatchGroup)
class MatchGroupAdmin(admin.ModelAdmin):

    list_display = ('id', 'name', 'scenario', 'number_playing_teams', 'num_pools', 'list_teams', 'matches_generated')
    list_filter = ('matches_generated', 'num_pools', 'number_playing_teams')
    actions = ['mark_finished', 'generate_matches']

    def list_teams(self, obj):
        return ', '.join([_.name for _ in obj.teams.all()])

    def mark_finished(self, request, queryset):
        for m in queryset:
            m.finish_matches()

    def generate_matches(self, request, queryset):
        for m in queryset:
            m.generate_matches()

    generate_matches.short_description = 'Generate matches for the selected groups'
    mark_finished.short_description = 'Mark related matches as finished'
