# Generated by Django 2.0 on 2017-12-26 17:46

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('leaderboard', '0003_auto_20171226_1655'),
    ]

    operations = [
        migrations.AddField(
            model_name='team',
            name='valid_buildscript',
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name='team',
            name='valid_repo',
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name='team',
            name='valid_startscript',
            field=models.BooleanField(default=False),
        ),
    ]
