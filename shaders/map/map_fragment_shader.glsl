 #version 330 core

#define NUM_TEXTURES 12

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D textures[NUM_TEXTURES];

out vec4 FragColor;

void main()
{     //middle junction corner
      int x = int(vPos.x),    x2048 = x % 2048,
          z = int(vPos.z),    z2048 = z % 2048;

      if          (z2048 >= 1023 && z2048 < 1024 && x2048 >= 1023 && x2048 < 1024)  FragColor = (texelFetch(textures[1] , ivec2(0, 511), 0) + texelFetch(textures[4]  , ivec2(511, 511), 0)  +
                                                                                                 texelFetch(textures[7] , ivec2(0, 0)  , 0) + texelFetch(textures[10] , ivec2(511, 0)  , 0)) / 4;
      //bottom junction corner
      else if     (z2048 >= 511  && z2048 < 512  && x2048 >= 1023 && x2048 < 1024)  FragColor = (texelFetch(textures[0] , ivec2(0  , 511), 0) + texelFetch(textures[1], ivec2(0  , 0), 0)  +
                                                                                                 texelFetch(textures[3] , ivec2(511, 511), 0) + texelFetch(textures[4], ivec2(511, 0), 0)) / 4;
      //top    junction corner
      else if     (z2048 >= 1535 && z2048 < 1536 && x2048 >= 1023 && x2048 < 1024)  FragColor = (texelFetch(textures[7] , ivec2(0  , 511), 0) + texelFetch(textures[9] , ivec2(0, 0)  , 0)  +
                                                                                                 texelFetch(textures[10], ivec2(511, 511), 0) + texelFetch(textures[11], ivec2(511, 0), 0)) / 4;
      //right  junction corner
      else if     (z2048 >= 1023 && z2048 < 1024 && x2048 >= 511  && x2048 < 512)   FragColor = (texelFetch(textures[2] , ivec2(0, 511), 0) + texelFetch(textures[1], ivec2(511, 511), 0)  +
                                                                                                 texelFetch(textures[6] , ivec2(0, 0)  , 0) + texelFetch(textures[7], ivec2(511, 0)  , 0)) / 4;
      //left   junction corner
      else if     (z2048 >= 1023 && z2048 < 1024 && x2048 >= 1535 && x2048 < 1536)  FragColor = (texelFetch(textures[4] , ivec2(0, 511), 0) + texelFetch(textures[5], ivec2(511, 511), 0)  +
                                                                                                 texelFetch(textures[10], ivec2(0, 0)  , 0) + texelFetch(textures[9], ivec2(511, 0)  , 0)) / 4;
      else if (z2048 % 2048 < 1024 && z2048 >= 1023)//xJunctions
      {
          int xCoord = int(TexCoords.x * 512.0f);
          
          if     (x2048 < 512)  FragColor = mix(texelFetch(textures[2], ivec2(xCoord, 511), 0), texelFetch(textures[6] , ivec2(xCoord, 0), 0), 0.5);
          else if(x2048 < 1024) FragColor = mix(texelFetch(textures[1], ivec2(xCoord, 511), 0), texelFetch(textures[7] , ivec2(xCoord, 0), 0), 0.5);
          else if(x2048 < 1536) FragColor = mix(texelFetch(textures[4], ivec2(xCoord, 511), 0), texelFetch(textures[10], ivec2(xCoord, 0), 0), 0.5);
          else if(x2048 < 2048) FragColor = mix(texelFetch(textures[5], ivec2(xCoord, 511), 0), texelFetch(textures[9] , ivec2(xCoord, 0), 0), 0.5);
          else                FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }
      else if (int(vPos.x) % 2048 < 1024 && int(vPos.x) % 2048 >= 1023)//yJunctions
      {
          int yCoord = int(TexCoords.y * 512.0f);

          if     (z2048 < 512)   FragColor = mix(texelFetch(textures[0], ivec2(0, yCoord), 0), texelFetch(textures[3] , ivec2(511, yCoord), 0), 0.5);
          else if(z2048 < 1024)  FragColor = mix(texelFetch(textures[1], ivec2(0, yCoord), 0), texelFetch(textures[4] , ivec2(511, yCoord), 0), 0.5);
          else if(z2048 < 1536)  FragColor = mix(texelFetch(textures[7], ivec2(0, yCoord), 0), texelFetch(textures[10], ivec2(511, yCoord), 0), 0.5);
          else if(z2048 < 2048)  FragColor = mix(texelFetch(textures[8], ivec2(0, yCoord), 0), texelFetch(textures[11], ivec2(511, yCoord), 0), 0.5);
          else                      FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }
      else FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}