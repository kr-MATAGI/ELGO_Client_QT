# JSON Schema

## ELGO_Control <-> Server (Contents Server)
  * 필요한 Schema는 [JohnnyJuheonShin/document](https://github.com/JohnnyJuheonShin/document) 혹은 구글 공유 드라이브에 대부분 있다.
  * 아래에 작성할 JSON Schema는 가장 데이터가 많은 Single Play / Play Schedule 로 전달되는 PlayData가 포함된 JSON의 Schema이다.
  
---
  
### Custom Play Data
  + Default
    <pre><code>
      {
        pld_name: String,
        pld_memo: String,
        pld_width: Integer,
        pld_orientation: String (landscape | portrait),
        page_data: [{
          pgd_duration: Integer (unit : sec),
          layer_data: [{
            ld_top: Integer,
            ld_left: Integer,
            ld_width: Integer,
            ld_height: Integer,
            ld_content: {
              type: String ( ( file | widget)/ (image | video | clock | date | weather | news) ),
              name: String,
              duration : float (type == video)
            }
          }]
        }],
        subtitle_data: [{
          sd_top: Integer,
          sd_left: Integer,
          sd_width: Integer,
          sd_height: Integer,
          sd_text: String,
          sd_fixed: Integer,
          sd_direction: String ( left | right | up | down ),
          sd_orientation: String ( landscape | portrait ),
          sd_behavior: String ( scroll | alternate ),
          sd_speed: Integer,
          sd_background_color: String (e.g. #ffffff),
          sd_font_color: String (e.g. #ffffff),
          sd_font_size: Integer
        }]
      }
    </code></pre>


***

### Fixed Play Data
  + Default
    <pre><code>

    </code></pre>
    
---

### Widget
  + Clock (Tab Name: 시계)
    <pre><code>
    
    </code></pre>
    
  + DateTime (Tab Name: 날짜)
    <pre><code>
    
    </code></pre>
    
  + Weather (Tab Name: 날씨)
    <pre><code>
    
    </code></pre>
    
  + News (Tab Name: 뉴스)
