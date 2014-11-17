/*
 Copyright 2014 OpenMarket Ltd
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#import <Foundation/Foundation.h>

#import "MXEvent.h"
#import "MXJSONModels.h"
#import "MXRoomMember.h"
#import "MXEventListener.h"
#import "MXRoomState.h"

@class MXSession;

/**
 Block called when an event of the registered types has been handled by the `MXRoom` instance.
 This is a specialisation of the `MXEventListenerBlock`.
 
 @param room the `MXRoom` that handled the event.
 @param event the new event.
 @param isLive YES if it is new event.
 */
typedef void (^MXRoomEventListenerBlock)(MXRoom *room, MXEvent *event, BOOL isLive);

/**
 `MXRoom` is the class
 */
@interface MXRoom : NSObject

/**
 A copy of the list of messages (MXEvent instances) currently loaded for this room.
 A message is either a non-state or a state event that is intended to be 
 displayed in a room chat history.
 The order is chronological: the first item is the oldest message retrieved so far.
 */
@property (nonatomic, readonly) NSArray *messages;

/**
 The last message.
 */
@property (nonatomic, readonly) MXEvent *lastMessage;

/**
 The uptodate state of the room.
 */
@property (nonatomic, readonly) MXRoomState *state;

/**
 Flag indicating if there are still events (in the past) to get with paginateBackMessages.
 */
@property (nonatomic, readonly) BOOL canPaginate;


- (id)initWithRoomId:(NSString*)room_id andMatrixSession:(MXSession*)mxSession;

- (id)initWithRoomId:(NSString*)room_id andMatrixSession:(MXSession*)mxSession andJSONData:(NSDictionary*)JSONData;

- (void)handleMessages:(MXPaginationResponse*)roomMessages
          isLiveEvents:(BOOL)isLiveEvents
             direction:(BOOL)direction;

- (void)handleStateEvents:(NSArray*)roomStateEvents;

/**
 Handle an event (message or state) that comes from the events streaming.
 
 @param event the event to handle.
 */
- (void)handleLiveEvent:(MXEvent*)event;

/**
 Get more messages from the past.
 The MXRoom `messages` property will be updated in case of successful response.
 
 @param numItems the number of items to get.
 @param success A block object called when the operation succeeds. It provides an array of retrieved
                `MXEvent` objects where the first item is the the oldest MXEvent in the array.
 @param failure A block object called when the operation fails.
 */
- (void)paginateBackMessages:(NSUInteger)numItems
                     success:(void (^)(NSArray *messages))success
                     failure:(void (^)(NSError *error))failure;


/**
 Register a listener for some types of events.
 
 To get only notifications for events that modify the `messages` property, use 
 mxSession.eventsFilterForMessages as types parameter.
 
 @param types an array of event types strings (MXEventTypeString). nil to listen to all events.
 @param listenerBlock the block that will called once a new event has been handled.
 @return a reference to use to unregister the listener
 */
- (id)registerEventListenerForTypes:(NSArray*)types block:(MXRoomEventListenerBlock)listenerBlock;

/**
 Unregister a listener.
 
 @param listener the reference of the listener to remove.
 */
- (void)unregisterListener:(id)listener;

/**
 Unregister all listeners.
 */
- (void)unregisterAllListeners;

@end