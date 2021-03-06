//
//  TODynamicPagingView.h
//
//  Copyright 2018-2020 Timothy Oliver. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to
//  deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
//  sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
//  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class TODynamicPagingView;

//-------------------------------------------------------------------
/** An enumeration of directions in which the scroll view may display pages. */
typedef enum {
    TODynamicPagingViewDirectionLeftToRight = 0, /** Pages ascend from the left, to the right */
    TODynamicPagingViewDirectionRightToLeft = 1 /** Pages ascend from the right, to the left */
} TODynamicPagingViewDirection;

//-------------------------------------------------------------------

/** Optional protocol that page views may implement. */
@protocol TODynamicPagingViewPageProtocol <NSObject>

@optional

/**
 A unique string value that can be used to let the pager view
 dequeue pre-made objects with the same identifier, or if pre-registered,
 create new instances automatically on request.
 
 If this property is not overridden, the page will be treated as the default
 type that will be returned whenever the identifier is nil.
 */
+ (NSString *)pageIdentifier;

/**
 A globally unique identifier that can be used to uniquely tag this specific
 page object. This can be used to retrieve the page from the pager view at a later
 time.
 */
- (NSString *)uniqueIdentifier;

/**
 Called just before the page object is removed from the visible page set,
 and re-enqueud by the data source.
 
 Use this method to return the page to a default state, and to clear out any
 references to memory-heavy objects like images.
 */
- (void)prepareForReuse;

@end

//-------------------------------------------------------------------

@protocol TODynamicPagingViewDataSource <NSObject>

@required

/** Called once upon each reload, the initial page view that will be displayed intially. */
- (nullable __kindof UIView *)initialPageViewForDynamicPagingView:(TODynamicPagingView *)dynamicPagingView;

/** Using the current page, fetch and return the next page view that will be displayed after it. */
- (nullable __kindof  UIView *)dynamicPagingView:(TODynamicPagingView *)dynamicPagingView
           nextPageViewAfterPageView:(__kindof UIView *)currentPageView;

/** Using the current page, fetch and return the previous page view that will be  */
- (nullable __kindof  UIView *)dynamicPagingView:(TODynamicPagingView *)dynamicPagingView
      previousPageViewBeforePageView:(__kindof UIView *)currentPageView;

@end

//-------------------------------------------------------------------

@protocol TODynamicPagingViewDelegate <NSObject>

@optional

@end

//-------------------------------------------------------------------

@interface TODynamicPagingView : UIView

/** Direct access to the scroll view object inside this view (read-only). */
@property (nonatomic, strong, readonly) UIScrollView *scrollView;

/** Data source object to supply page information to the scroll view */
@property (nonatomic, weak, nullable) id <TODynamicPagingViewDataSource> dataSource;

/** Delegate object in which page scroll view events are sent. */
@property (nonatomic, weak, nullable) id <TODynamicPagingViewDelegate> delegate;

/** Width of the spacing between pages in points (default value of 40). */
@property (nonatomic, assign) CGFloat pageSpacing;

/** The direction of the layout order of pages. */
@property (nonatomic, assign) TODynamicPagingViewDirection pageScrollDirection;

/* All of the page view objects currently placed in the scroll view. */
@property (nonatomic, readonly) NSArray<UIView *> *visiblePages;

/**
 Registers a page view class that can be automatically instantiated as needed.
 If the class overrides `pageIdentifier`, new instances may automatically be created
 when needed. Any classes that do not override that property will become the default
 page class.
 */
- (void)registerPageViewClass:(Class)pageViewClass;

/** Reload the view from scratch and re-layout all pages. */
- (void)reload;

/** Perform a check in needing to add a previous or next page that didn't previously exist. */
- (void)setNeedsPageUpdate;

/** Returns a page view from the default queue of pages, ready for re-use. */
- (nullable __kindof UIView *)dequeueReusablePageView;

/** Returns a page view from the specific queue matching the provided identifier string. */
- (nullable __kindof UIView *)dequeueReusablePageViewForIdentifier:(nullable NSString *)identifier;

/** The currently visible primary page view on screen. */
- (nullable __kindof UIView *)currentPageView;

/** The next page after the currently visible page on the screen. */
- (nullable __kindof UIView *)nextPageView;

/** The previous page before the currently visible page on the screen. */
- (nullable __kindof UIView *)previousPageView;

/** Returns the visible page view for the supplied unique identifier, or nil otherwise. */
- (nullable __kindof UIView *)pageViewForUniqueIdentifier:(NSString *)identifier;

/** Advance one page to the left (Regardless of current scroll direction) */
- (void)turnToLeftPageAnimated:(BOOL)animated;

/** Advance one page to the right (Regardless of current scroll direction) */
- (void)turnToRightPageAnimated:(BOOL)animated;

/** Jump ahead to an arbitry next page view, using the provided block to generate the page. */
- (void)jumpToNextPageAnimated:(BOOL)animated
                     withBlock:(UIView * (^)(TODynamicPagingView *dynamicPagingView, UIView *currentView))pageBlock;

/** Jump backwards to an arbitrary previous page view, using the provided block to generate the page. */
- (void)jumpToPreviousPageAnimated:(BOOL)animated
                         withBlock:(UIView * (^)(TODynamicPagingView *dynamicPagingView, UIView *currentView))pageBlock;

@end

NS_ASSUME_NONNULL_END
