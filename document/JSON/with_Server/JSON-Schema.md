# JSON Schema

## ELGO_Control <-> Server (Contents Server)
  * 필요한 Schema는 [JohnnyJuheonShin/document](https://github.com/JohnnyJuheonShin/document) 혹은 구글 공유 드라이브(서진성 더미)에 대부분 있다.
  * 아래에 작성할 JSON Schema는 가장 데이터가 많은 Single Play / Play Schedule 로 전달되는 PlayData가 포함된 JSON의 Schema이다.
  * 주의 - JSON Parsing의 동작에서 아래의 Object tag name 혹은 value의 명칭이 완전히 일치하지 않는다.
---
  
### Custom Play Data
  + Default
    <pre><code>
      {
        "pld_type": String ( custom | fixed ),
        "pld_name": String,
        "pld_memo": String,
        "pld_width": Integer,
        "pld_height": Integer,
        "pld_orientation": String (landscape | portrait),
        "page_data": [{
          "pgd_duration": Integer (unit : sec),
          "layer_data": [{
            "ld_top": Integer,
            "ld_left": Integer,
            "ld_width": Integer,
            "ld_height": Integer,
            "ld_content": {
              "type": String ( ( file | widget) / (image | video | clock | date | weather | news) ),
              "name": String,
              "duration": float (type == video)
            }
          }]
        }],
        "subtitle_data": [{
          "sd_top": Integer,
          "sd_left": Integer,
          "sd_width": Integer,
          "sd_height": Integer,
          "sd_text": String,
          "sd_fixed": Integer ( 0 | 1 ),
          "sd_direction": String ( left | right | up | down ),
          "sd_orientation": String ( landscape | portrait ),
          "sd_behavior": String ( scroll | alternate ),
          "sd_speed": Integer,
          "sd_background_color": String (e.g. #ffffff),
          "sd_font_color": String (e.g. #ffffff),
          "sd_font_size": Integer
        }]
      }
    </code></pre>


***

### Fixed Play Data
  + Default
    <pre><code>
     {
       "pld_type": String ( custom | fixed ),
       "pld_name": String,
       "pld_memo": String,
       "pld_width": Integer,
       "pld_height": Integer,
       "pld_orientation": String (landscape | portrait),
       "layer_data": [{
         "content_data": [{
           "cd_type": String ( ( file | widget) / (image | video | clock | date | weather | news) ),
           "cd_name": String,
           "cd_duration": Integer (unit : sec)
         }],
       "ld_left": Integer,
       "ld_top": Integer,
       "ld_width": Integer,
       "ld_height": Integer
       }],
       "subtitle_data": [{
         "subtitle_data_id": Integer (Not Using),
         "sd_left": Integer,
         "sd_top": Integer,
         "sd_width": Integer,
         "sd_height": Integer,
         "sd_text": String,
         "sd_fixed": Integer ( 0 | 1 ),
         "sd_direction": String ( left | right | up | down ),
         "sd_orientation": String ( landscape | portrait ),
         "sd_behavior": String ( scroll | alternate ),
         "sd_speed": Integer,
         "sd_background_color": String (e.g. #ffffff),
         "sd_font_color": String (e.g. #ffffff),
         "sd_font_size": String
       }]
     }

    </code></pre>
    
---

### Widget - Content Data 안에 설정되는 JSON Data.

  + Clock (Tab Name: 시계)
    <pre><code>
     {
       "type": "widget/clock",
       "hour_type": String ( 12h | 24h ),
       "color": String (e.g. #ffffff),
       "bg_color": String (e.g. #ffffff),
       "bg_opacity": Boolean
     }
    </code></pre>
    
  + DateTime (Tab Name: 날짜)
    <pre><code>
     {
       "type": "widget/date",
       "date_type": String ( yes | no ),
       "color": String (e.g. #ffffff),
       "bg_color": String (e.g. #ffffff),
       "bg_opacity": Boolean
     }
    </code></pre>
    
  + Weather (Tab Name: 날씨)
    <pre><code>
     {
       "type": "widget/weather",
       "nx": Integer (for new open api),
       "ny": Integer (for new open api),
       "area": String (value is Integer),
       "area_name": String,
       "area2": String (value is Integer),
       "area2_name": String,
       "color": String (e.g. #ffffff),
       "bg_color": String (e.g. #ffffff),
       "bg_opacity": Boolean
     }
    </code></pre>
    
  + News (Tab Name: 뉴스)
    <pre><code>
      {
        "type": "widget/news",
        "category": String ( browse | headline | politics | economy | society | local | international | culture | sports | weather ),
        "news_count": String (value is Integer),
        "font_size": String (value is Integer),
        "color": String (e.g. #ffffff),
        "news_bg_color": String (e.g. #ffffff),
        "news_bg_opacity": Boolean,
        "bg_color": String (e.g. #ffffff),
        "bg_opacity": Booelan
      }
    </code><pre>
