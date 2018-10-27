"""webagenda URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from webagenda import views
from rest_framework_jwt.views import obtain_jwt_token

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/v1/login', views.user_login),
    path('api/v1/logout', views.user_logout),
    path('api/v1/register', views.user_register),
    path('api/v1/create_meeting', views.create_meeting),
    path('api/v1/users', views.list_users),
    path('api/v1/users/<path:username>', views.delete_user),
    # path('api/v1/delete/all_meetings', views.delete_all_meetings),
    path('api/v1/meetings/sponsor/<path:title>', views.delete_meeting),
    path('api/v1/meetings/sponsor', views.list_sponsor_meetings),
    path('api/v1/meetings/participate', views.list_participate_meetings),
    path('api/v1/meeting/<path:title>', views.query_meeting_by_title),
    path('api/v1/meeting', views.query_meeting_by_date),
    path('api/v1/token-auth', obtain_jwt_token),
]
