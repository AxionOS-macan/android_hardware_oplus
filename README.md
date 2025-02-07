# hardware/oplus

## Soong options

| Namespace | Variable | Description | Default |
| --------- | -------- | ----------- | ------- |
| OPLUS_LINEAGE_LIVEDISPLAY_HAL | ENABLE_AB | Enable AdaptiveBacklight feature | false |
| OPLUS_LINEAGE_LIVEDISPLAY_HAL | ENABLE_AF | Enable AntiFlicker feature | false |
| OPLUS_LINEAGE_LIVEDISPLAY_HAL | ENABLE_DM | Enable DisplayModes feature | false |
| OPLUS_LINEAGE_LIVEDISPLAY_HAL | ENABLE_PA | Enable PictureAdjustment feature | true |
| OPLUS_LINEAGE_LIVEDISPLAY_HAL | ENABLE_SE | Enable SunlightEnhancement feature | true |
| OPLUS_LINEAGE_TOUCH_HAL | ENABLE_GM | Enable GloveMode feature | false |
| OPLUS_LINEAGE_TOUCH_HAL | ENABLE_HTPR | Enable HighTouchPollingRate feature | true |
| OPLUS_LINEAGE_TOUCH_HAL | ENABLE_TG | Enable TouchscreenGesture feature | true |
| OPLUS_LINEAGE_TOUCH_HAL | HTPR_ENABLE_NODE | HighTouchPollingRate enable node (OplusTouchConstants member) | Auto-detect |
| OPLUS_LINEAGE_TOUCH_HAL | HTPR_ENABLE_VALUE | HighTouchPollingRate enable value | From sys.touch.report_rate / detected node |
| OPLUS_LINEAGE_TOUCH_HAL | INCLUDE_DIR | Device specific include dir path | |
| OPLUS_LINEAGE_TOUCH_HAL | USE_OPLUSTOUCH | Use and interface with stock OplusTouch | false |
| OPLUS_LINEAGE_VIBRATOR_HAL | INCLUDE_DIR | Device specific include dir path | |
| OPLUS_LINEAGE_VIBRATOR_HAL | USE_EFFECT_STREAM | Enable effect stream feature | false |
| OPLUS_SENSORS_FUSIONLIGHT | BACKEND_SUBHAL_LIB_NAME | FusionLight Sensors SubHAL backend library name | |

When neither `HTPR_ENABLE_NODE` nor `HTPR_ENABLE_VALUE` is set, HighTouchPollingRate
keeps automatic node detection and uses `sys.touch.report_rate` (default mode `3`).
Setting either option overrides this behavior for IOplusTouch: an omitted node
uses `GAME_SWITCH_ENABLE_NODE`, and an omitted enable value uses `1`. Disabling
writes `0`. The proc fallback always writes `1` to enable and `0` to disable.

For example, to use node 182 with a fixed value of 360:

```make
$(call soong_config_set,OPLUS_LINEAGE_TOUCH_HAL,HTPR_ENABLE_NODE,REPORT_RATE_NODE)
$(call soong_config_set,OPLUS_LINEAGE_TOUCH_HAL,HTPR_ENABLE_VALUE,360)
```
