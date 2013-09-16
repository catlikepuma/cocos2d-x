/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "UIButton.h"
#include "../../../GUI/CCControlExtension/CCScale9Sprite.h"

NS_CC_EXT_BEGIN

#define NORMALRENDERERZ (0)
#define PRESSEDRENDERERZ (0)
#define DISABLEDRENDERERZ (0)
#define TITLERENDERERZ (1)
    
UIButton::UIButton():
m_pButtonNormalRenderer(NULL),
m_pButtonClickedRenderer(NULL),
m_pButtonDisableRenderer(NULL),
m_pTitleRenderer(NULL),
m_strNormalFileName(""),
m_strClickedFileName(""),
m_strDisabledFileName(""),
m_bPrevIgnoreSize(true),
m_bScale9Enabled(false),
m_capInsetsNormal(Rect::ZERO),
m_capInsetsPressed(Rect::ZERO),
m_capInsetsDisabled(Rect::ZERO),
m_eNormalTexType(UI_TEX_TYPE_LOCAL),
m_ePressedTexType(UI_TEX_TYPE_LOCAL),
m_eDisabledTexType(UI_TEX_TYPE_LOCAL),
m_normalTextureSize(_size),
m_pressedTextureSize(_size),
m_disabledTextureSize(_size),
m_bPressedActionEnabled(false),
m_titleColor(Color3B::WHITE)
{
    
}

UIButton::~UIButton()
{
}

UIButton* UIButton::create()
{
    UIButton* widget = new UIButton();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UIButton::init()
{
    if (UIWidget::init())
    {
        return true;
    }
    return false;
}

void UIButton::initRenderer()
{
    UIWidget::initRenderer();
    m_pButtonNormalRenderer = CCSprite::create();
    m_pButtonClickedRenderer = CCSprite::create();
    m_pButtonDisableRenderer = CCSprite::create();
    m_pTitleRenderer = CCLabelTTF::create();
    _renderer->addChild(m_pButtonNormalRenderer,NORMALRENDERERZ);
    _renderer->addChild(m_pButtonClickedRenderer,PRESSEDRENDERERZ);
    _renderer->addChild(m_pButtonDisableRenderer,DISABLEDRENDERERZ);
    _renderer->addChild(m_pTitleRenderer,TITLERENDERERZ);
}

void UIButton::setScale9Enabled(bool able)
{
    if (m_bScale9Enabled == able)
    {
        return;
    }
    _brightStyle = BRIGHT_NONE;
    m_bScale9Enabled = able;

    
    _renderer->removeChild(m_pButtonNormalRenderer, true);
    _renderer->removeChild(m_pButtonClickedRenderer, true);
    _renderer->removeChild(m_pButtonDisableRenderer, true);
    
    m_pButtonNormalRenderer = NULL;
    m_pButtonClickedRenderer = NULL;
    m_pButtonDisableRenderer = NULL;
    if (m_bScale9Enabled)
    {
        m_pButtonNormalRenderer = Scale9Sprite::create();
        m_pButtonClickedRenderer = Scale9Sprite::create();
        m_pButtonDisableRenderer = Scale9Sprite::create();
    }
    else
    {
        m_pButtonNormalRenderer = CCSprite::create();
        m_pButtonClickedRenderer = CCSprite::create();
        m_pButtonDisableRenderer = CCSprite::create();
    }

    loadTextureNormal(m_strNormalFileName.c_str(), m_eNormalTexType);
    loadTexturePressed(m_strClickedFileName.c_str(), m_ePressedTexType);
    loadTextureDisabled(m_strDisabledFileName.c_str(), m_eDisabledTexType);
    _renderer->addChild(m_pButtonNormalRenderer,NORMALRENDERERZ);
    _renderer->addChild(m_pButtonClickedRenderer,PRESSEDRENDERERZ);
    _renderer->addChild(m_pButtonDisableRenderer,DISABLEDRENDERERZ);
    if (m_bScale9Enabled)
    {
        bool ignoreBefore = _ignoreSize;
        ignoreContentAdaptWithSize(false);
        m_bPrevIgnoreSize = ignoreBefore;
    }
    else
    {
        ignoreContentAdaptWithSize(m_bPrevIgnoreSize);
    }
    setCapInsetsNormalRenderer(m_capInsetsNormal);
    setCapInsetsPressedRenderer(m_capInsetsPressed);
    setCapInsetsDisabledRenderer(m_capInsetsDisabled);
    setBright(_bright);
}

void UIButton::ignoreContentAdaptWithSize(bool ignore)
{
    if (!m_bScale9Enabled || (m_bScale9Enabled && !ignore))
    {
        UIWidget::ignoreContentAdaptWithSize(ignore);
        m_bPrevIgnoreSize = ignore;
    }
}

void UIButton::loadTextures(const char* normal,const char* selected,const char* disabled,TextureResType texType)
{
    loadTextureNormal(normal,texType);
    loadTexturePressed(selected,texType);
    loadTextureDisabled(disabled,texType);
}

void UIButton::loadTextureNormal(const char* normal,TextureResType texType)
{
    if (!normal || strcmp(normal, "") == 0)
    {
        return;
    }
    m_strNormalFileName = normal;
    m_eNormalTexType = texType;
    if (m_bScale9Enabled)
    {
        switch (m_eNormalTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->initWithFile(normal);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->initWithSpriteFrameName(normal);
                break;
            default:
                break;
        }
        dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->setColor(getColor());
        dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->setOpacity(getOpacity());
    }
    else
    {
        switch (m_eNormalTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->initWithFile(normal);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->initWithSpriteFrameName(normal);
                break;
            default:
                break;
        }
        dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->setColor(getColor());
        dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->setOpacity(getOpacity());
    }
    m_normalTextureSize = m_pButtonNormalRenderer->getContentSize();
    updateAnchorPoint();
    normalTextureScaleChangedWithSize();
}

void UIButton::loadTexturePressed(const char* selected,TextureResType texType)
{
    if (!selected || strcmp(selected, "") == 0)
    {
        return;
    }
    m_strClickedFileName = selected;
    m_ePressedTexType = texType;
    if (m_bScale9Enabled)
    {
        switch (m_ePressedTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->initWithFile(selected);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->initWithSpriteFrameName(selected);
                break;
            default:
                break;
        }
        dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->setColor(getColor());
        dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->setOpacity(getOpacity());
    }
    else
    {
        switch (m_ePressedTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->initWithFile(selected);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->initWithSpriteFrameName(selected);
                break;
            default:
                break;
        }
        dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->setColor(getColor());
        dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->setOpacity(getOpacity());
    }
    m_pressedTextureSize = m_pButtonClickedRenderer->getContentSize();
    updateAnchorPoint();
    pressedTextureScaleChangedWithSize();
}

void UIButton::loadTextureDisabled(const char* disabled,TextureResType texType)
{
    if (!disabled || strcmp(disabled, "") == 0)
    {
        return;
    }
    m_strDisabledFileName = disabled;
    m_eDisabledTexType = texType;
    if (m_bScale9Enabled)
    {
        switch (m_eDisabledTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->initWithFile(disabled);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->initWithSpriteFrameName(disabled);
                break;
            default:
                break;
        }
        dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->setColor(getColor());
        dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->setOpacity(getOpacity());
    }
    else
    {
        switch (m_eDisabledTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->initWithFile(disabled);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->initWithSpriteFrameName(disabled);
                break;
            default:
                break;
        }
        dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->setColor(getColor());
        dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->setOpacity(getOpacity());
    }
    m_disabledTextureSize = m_pButtonDisableRenderer->getContentSize();
    updateAnchorPoint();
    disabledTextureScaleChangedWithSize();
}

void UIButton::setCapInsets(const Rect &capInsets)
{
    setCapInsetsNormalRenderer(capInsets);
    setCapInsetsPressedRenderer(capInsets);
    setCapInsetsDisabledRenderer(capInsets);
}

void UIButton::setCapInsetsNormalRenderer(const Rect &capInsets)
{
    m_capInsetsNormal = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->setCapInsets(capInsets);
}

void UIButton::setCapInsetsPressedRenderer(const Rect &capInsets)
{
    m_capInsetsPressed = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->setCapInsets(capInsets);
}

void UIButton::setCapInsetsDisabledRenderer(const Rect &capInsets)
{
    m_capInsetsDisabled = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->setCapInsets(capInsets);
}

void UIButton::onPressStateChangedToNormal()
{
    m_pButtonNormalRenderer->setVisible(true);
    m_pButtonClickedRenderer->setVisible(false);
    m_pButtonDisableRenderer->setVisible(false);
    if (m_bPressedActionEnabled)
    {
        m_pButtonNormalRenderer->stopAllActions();
        m_pButtonClickedRenderer->stopAllActions();
        m_pButtonDisableRenderer->stopAllActions();
        Action *zoomAction = ScaleTo::create(0.05f, 1.0f);
        Action *zoomAction1 = ScaleTo::create(0.05f, 1.0f);
        Action *zoomAction2 = ScaleTo::create(0.05f, 1.0f);
        m_pButtonNormalRenderer->runAction(zoomAction);
        m_pButtonClickedRenderer->runAction(zoomAction1);
        m_pButtonDisableRenderer->runAction(zoomAction2);
    }
}

void UIButton::onPressStateChangedToPressed()
{
    m_pButtonNormalRenderer->setVisible(false);
    m_pButtonClickedRenderer->setVisible(true);
    m_pButtonDisableRenderer->setVisible(false);
    if (m_bPressedActionEnabled)
    {
        m_pButtonNormalRenderer->stopAllActions();
        m_pButtonClickedRenderer->stopAllActions();
        m_pButtonDisableRenderer->stopAllActions();
        Action *zoomAction = ScaleTo::create(0.05f, 1.1f);
        Action *zoomAction1 = ScaleTo::create(0.05f, 1.1f);
        Action *zoomAction2 = ScaleTo::create(0.05f, 1.1f);
        m_pButtonNormalRenderer->runAction(zoomAction);
        m_pButtonClickedRenderer->runAction(zoomAction1);
        m_pButtonDisableRenderer->runAction(zoomAction2);
    }
}

void UIButton::onPressStateChangedToDisabled()
{
    m_pButtonNormalRenderer->setVisible(false);
    m_pButtonClickedRenderer->setVisible(false);
    m_pButtonDisableRenderer->setVisible(true);
}

void UIButton::setFlipX(bool flipX)
{
    m_pTitleRenderer->setFlippedX(flipX);
    if (m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->setFlippedX(flipX);
    dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->setFlippedX(flipX);
    dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->setFlippedX(flipX);
}

void UIButton::setFlipY(bool flipY)
{
    m_pTitleRenderer->setFlippedY(flipY);
    if (m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->setFlippedY(flipY);
    dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->setFlippedY(flipY);
    dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->setFlippedY(flipY);
}

bool UIButton::isFlipX()
{
    if (m_bScale9Enabled)
    {
        return false;
    }
    return dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->isFlippedX();
}

bool UIButton::isFlipY()
{
    if (m_bScale9Enabled)
    {
        return false;
    }
    return dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->isFlippedY();
}

void UIButton::setAnchorPoint(const Point &pt)
{
    UIWidget::setAnchorPoint(pt);
    m_pButtonNormalRenderer->setAnchorPoint(pt);
    m_pButtonClickedRenderer->setAnchorPoint(pt);
    m_pButtonDisableRenderer->setAnchorPoint(pt);
    m_pTitleRenderer->setPosition(Point(_size.width*(0.5f-_anchorPoint.x), _size.height*(0.5f-_anchorPoint.y)));
}

void UIButton::setNormalSpriteFrame(SpriteFrame *frame)
{
    if (!frame)
    {
        return;
    }
    if (m_bScale9Enabled)
    {
        dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->setSpriteFrame(frame);
    }
    else
    {
        dynamic_cast<Sprite*>(m_pButtonNormalRenderer)->setDisplayFrame(frame);
    }
}

void UIButton::setPressedSpriteFrame(SpriteFrame *frame)
{
    if (!frame)
    {
        return;
    }
    if (m_bScale9Enabled)
    {
        dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->setSpriteFrame(frame);
    }
    else
    {
        dynamic_cast<Sprite*>(m_pButtonClickedRenderer)->setDisplayFrame(frame);
    }
}

void UIButton::setDisabledSpriteFrame(SpriteFrame *frame)
{
    if (!frame)
    {
        return;
    }
    if (m_bScale9Enabled)
    {
        dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->setSpriteFrame(frame);
    }
    else
    {
        dynamic_cast<Sprite*>(m_pButtonDisableRenderer)->setDisplayFrame(frame);
    }
}

void UIButton::onSizeChanged()
{
    normalTextureScaleChangedWithSize();
    pressedTextureScaleChangedWithSize();
    disabledTextureScaleChangedWithSize();
}

const Size& UIButton::getContentSize() const
{
    return m_normalTextureSize;
}

Node* UIButton::getVirtualRenderer()
{
    if (_bright)
    {
        switch (_brightStyle)
        {
            case BRIGHT_NORMAL:
                return m_pButtonNormalRenderer;
            case BRIGHT_HIGHLIGHT:
                return m_pButtonClickedRenderer;
            default:
                return NULL;
        }
    }
    else
    {
        return m_pButtonDisableRenderer;
    }
}

void UIButton::normalTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pButtonNormalRenderer->setScale(1.0f);
            _size = m_normalTextureSize;
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<Scale9Sprite*>(m_pButtonNormalRenderer)->setPreferredSize(_size);
        }
        else
        {
            Size textureSize = m_normalTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pButtonNormalRenderer->setScale(1.0f);
                return;
            }
            float scaleX = _size.width / textureSize.width;
            float scaleY = _size.height / textureSize.height;
            m_pButtonNormalRenderer->setScaleX(scaleX);
            m_pButtonNormalRenderer->setScaleY(scaleY);
        }
    }
}

void UIButton::pressedTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pButtonClickedRenderer->setScale(1.0f);
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<Scale9Sprite*>(m_pButtonClickedRenderer)->setPreferredSize(_size);
        }
        else
        {
            Size textureSize = m_pressedTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pButtonClickedRenderer->setScale(1.0f);
                return;
            }
            float scaleX = _size.width / m_pressedTextureSize.width;
            float scaleY = _size.height / m_pressedTextureSize.height;
            m_pButtonClickedRenderer->setScaleX(scaleX);
            m_pButtonClickedRenderer->setScaleY(scaleY);
        }
    }
}

void UIButton::disabledTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pButtonDisableRenderer->setScale(1.0f);
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<Scale9Sprite*>(m_pButtonDisableRenderer)->setPreferredSize(_size);
        }
        else
        {
            Size textureSize = m_disabledTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pButtonDisableRenderer->setScale(1.0f);
                return;
            }
            float scaleX = _size.width / m_disabledTextureSize.width;
            float scaleY = _size.height / m_disabledTextureSize.height;
            m_pButtonDisableRenderer->setScaleX(scaleX);
            m_pButtonDisableRenderer->setScaleY(scaleY);
        }
    }
}

void UIButton::setPressedActionEnabled(bool enabled)
{
    m_bPressedActionEnabled = enabled;
}

void UIButton::setTitleText(const char* text)
{
    m_pTitleRenderer->setString(text);
}

const char* UIButton::getTitleText() const
{
    return m_pTitleRenderer->getString();
}

void UIButton::setTitleColor(const Color3B& color)
{
    m_titleColor = color;
    m_pTitleRenderer->setColor(color);
}

const Color3B& UIButton::getTitleColor() const
{
    return m_pTitleRenderer->getColor();
}

void UIButton::setTitleFontSize(float size)
{
    m_pTitleRenderer->setFontSize(size);
}

float UIButton::getTitleFontSize() const
{
    return m_pTitleRenderer->getFontSize();
}

void UIButton::setTitleFontName(const char* fontName)
{
    m_pTitleRenderer->setFontName(fontName);
}

const char* UIButton::getTitleFontName() const
{
    return m_pTitleRenderer->getFontName();
}

void UIButton::setColor(const Color3B &color)
{
    UIWidget::setColor(color);
    setTitleColor(m_titleColor);
}

NS_CC_EXT_END