namespace HonorLogic
{
    internal enum MessageId
    {
        // ReSharper disable InconsistentNaming
        MSG_SHOW_MESSAGE = 1,
        MSG_UPDATE_LOCK_LIST = 2, //Не реализованно в браслете
        MSG_SET_PLAYER_NAME = 3,
        MSG_SET_PLAYER_REGENERATION = 4,
        MSG_ROOM_HIT = 8,
        MSG_GET_PLAYER_INFO = 12, //Не реализованно в браслете
        MSG_TASK_COMPLETED = 64,    //Не реализованно в браслете
        MSG_TASK_COMPLETION_CONFIRMED = 65, //Не реализованно в браслете
        // ReSharper restore InconsistentNaming
    }
}