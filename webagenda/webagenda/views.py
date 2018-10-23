from django.contrib.auth import login, logout, authenticate
from rest_framework.decorators import api_view,  authentication_classes, permission_classes
from rest_framework.authentication import SessionAuthentication
from rest_framework_jwt.authentication import JSONWebTokenAuthentication
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
import pyagenda


@api_view(['POST'])
def user_login(request):
    user = request.data['user']
    user = authenticate(
        request, username=user['username'], password=user['password'])
    if user:
        login(request, user)
        return Response({"success": True, "user": {
            "username": user.username
        }})
    else:
        return Response({"success": False})


@api_view(['POST'])
def user_register(request):
    user = request.data['user']
    success = pyagenda.register(**user)
    return Response({"success": success})


@api_view(['POST'])
def user_logout(request):
    logout(request)
    return Response({"success": True})


@api_view(['GET'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def list_users(request):
    users = pyagenda.list_all_users()
    return Response({"success": True, "users": users})


@api_view(['GET'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def query_meeting_by_title(request, title):
    meetings = pyagenda.query_meeting_by_title(request.user.username, title)
    return Response({"success": True, "meetings": meetings})


@api_view(['GET'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def query_meeting_by_date(request):
    start_date = request.query_params['start_date']
    end_date = request.query_params['end_date']
    meetings = pyagenda.query_meeting_by_date(
        request.user.username, start_date, end_date)
    return Response({"success": True, "meetings": meetings})


@api_view(['GET'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def list_sponsor_meetings(request):
    meetings = pyagenda.list_sponsor_meetings(request.user.username)
    return Response({"success": True, "meetings": meetings})


@api_view(['GET'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def list_participate_meetings(request):
    meetings = pyagenda.list_participate_meetings(request.user.username)
    return Response({"success": True, "meetings": meetings})


@api_view(['DELETE'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def delete_user(request):
    success = pyagenda.delete_user(request.user.username)
    logout(request)
    return Response({"success": success})


@api_view(['DELETE'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def delete_all_meetings(request):
    success = pyagenda.delete_all_meetings(request.user.username)
    return Response({"success": success})


@api_view(['DELETE'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def delete_meeting(request, title):
    success = pyagenda.delete_meeting(request.user.username, title)
    return Response({"success": success})


@api_view(['POST'])
@authentication_classes((SessionAuthentication, JSONWebTokenAuthentication))
@permission_classes((IsAuthenticated,))
def create_meeting(request):
    user = request.user
    username = user.username
    meeting = request.data['meeting']
    success = pyagenda.create_meeting(username=username, **meeting)
    return Response({"success": success})
