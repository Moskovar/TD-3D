 #version 330 core

#define NUM_TEXTURES 12
#define LARGETILE_SIZE 512

in vec2 TexCoords;
in vec3 vPos;
uniform sampler2D textures[NUM_TEXTURES];

out vec4 FragColor;

void main()
{     //middle junction corner
      int x = int(vPos.x),    x2048 = x % (LARGETILE_SIZE * 4),
          z = int(vPos.z),    z2048 = z % (LARGETILE_SIZE * 4);

      if          (z2048 >= ((LARGETILE_SIZE * 2) - 1) && z2048 < (LARGETILE_SIZE * 2) && x2048 >= ((LARGETILE_SIZE * 2) - 1) && x2048 < (LARGETILE_SIZE * 2))  FragColor = (texelFetch(textures[1] , ivec2(0, LARGETILE_SIZE - 1), 0) + texelFetch(textures[4]  , ivec2(LARGETILE_SIZE - 1, LARGETILE_SIZE - 1), 0)  +
                                                                                                 texelFetch(textures[7] , ivec2(0, 0)  , 0) + texelFetch(textures[10] , ivec2(LARGETILE_SIZE - 1, 0)  , 0)) / 4;
      //bottom junction corner
      else if     (z2048 >= LARGETILE_SIZE - 1  && z2048 < LARGETILE_SIZE  && x2048 >= ((LARGETILE_SIZE * 2) - 1) && x2048 < (LARGETILE_SIZE * 2))  FragColor = (texelFetch(textures[0] , ivec2(0  , LARGETILE_SIZE - 1), 0) + texelFetch(textures[1], ivec2(0  , 0), 0)  +
                                                                                                 texelFetch(textures[3] , ivec2(LARGETILE_SIZE - 1, LARGETILE_SIZE - 1), 0) + texelFetch(textures[4], ivec2(LARGETILE_SIZE - 1, 0), 0)) / 4;
      //top    junction corner
      else if     (z2048 >= ((LARGETILE_SIZE * 3) - 1) && z2048 < (LARGETILE_SIZE * 3) && x2048 >= ((LARGETILE_SIZE * 2) - 1) && x2048 < (LARGETILE_SIZE * 2))  FragColor = (texelFetch(textures[7] , ivec2(0  , LARGETILE_SIZE - 1), 0) + texelFetch(textures[9] , ivec2(0, 0)  , 0)  +
                                                                                                 texelFetch(textures[10], ivec2(LARGETILE_SIZE - 1, LARGETILE_SIZE - 1), 0) + texelFetch(textures[11], ivec2(LARGETILE_SIZE - 1, 0), 0)) / 4;
      //right  junction corner
      else if     (z2048 >= ((LARGETILE_SIZE * 2) - 1) && z2048 < (LARGETILE_SIZE * 2) && x2048 >= LARGETILE_SIZE - 1  && x2048 < LARGETILE_SIZE)   FragColor = (texelFetch(textures[2] , ivec2(0, LARGETILE_SIZE - 1), 0) + texelFetch(textures[1], ivec2(LARGETILE_SIZE - 1, LARGETILE_SIZE - 1), 0)  +
                                                                                                 texelFetch(textures[6] , ivec2(0, 0)  , 0) + texelFetch(textures[7], ivec2(LARGETILE_SIZE - 1, 0)  , 0)) / 4;
      //left   junction corner
      else if     (z2048 >= ((LARGETILE_SIZE * 2) - 1) && z2048 < (LARGETILE_SIZE * 2) && x2048 >= ((LARGETILE_SIZE * 3) - 1) && x2048 < (LARGETILE_SIZE * 3))  FragColor = (texelFetch(textures[4] , ivec2(0, LARGETILE_SIZE - 1), 0) + texelFetch(textures[5], ivec2(LARGETILE_SIZE - 1, LARGETILE_SIZE - 1), 0)  +
                                                                                                 texelFetch(textures[10], ivec2(0, 0)  , 0) + texelFetch(textures[9], ivec2(LARGETILE_SIZE - 1, 0)  , 0)) / 4;
      else if (z2048 % 2048 < (LARGETILE_SIZE * 2) && z2048 >= ((LARGETILE_SIZE * 2) - 1))//xJunctions
      {
          int xCoord = int(TexCoords.x * LARGETILE_SIZE);
          
          if     (x2048 < LARGETILE_SIZE)  FragColor = mix(texelFetch(textures[2], ivec2(xCoord, LARGETILE_SIZE - 1), 0), texelFetch(textures[6] , ivec2(xCoord, 0), 0), 0.5);
          else if(x2048 < (LARGETILE_SIZE * 2)) FragColor = mix(texelFetch(textures[1], ivec2(xCoord, LARGETILE_SIZE - 1), 0), texelFetch(textures[7] , ivec2(xCoord, 0), 0), 0.5);
          else if(x2048 < (LARGETILE_SIZE * 3)) FragColor = mix(texelFetch(textures[4], ivec2(xCoord, LARGETILE_SIZE - 1), 0), texelFetch(textures[10], ivec2(xCoord, 0), 0), 0.5);
          else if(x2048 < 2048) FragColor = mix(texelFetch(textures[5], ivec2(xCoord, LARGETILE_SIZE - 1), 0), texelFetch(textures[9] , ivec2(xCoord, 0), 0), 0.5);
          else                FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }
      else if (int(vPos.x) % 2048 < (LARGETILE_SIZE * 2) && int(vPos.x) % 2048 >= ((LARGETILE_SIZE * 2) - 1))//yJunctions
      {
          int yCoord = int(TexCoords.y * LARGETILE_SIZE);

          if     (z2048 < LARGETILE_SIZE)   FragColor = mix(texelFetch(textures[0], ivec2(0, yCoord), 0), texelFetch(textures[3] , ivec2(LARGETILE_SIZE - 1, yCoord), 0), 0.5);
          else if(z2048 < (LARGETILE_SIZE * 2))  FragColor = mix(texelFetch(textures[1], ivec2(0, yCoord), 0), texelFetch(textures[4] , ivec2(LARGETILE_SIZE - 1, yCoord), 0), 0.5);
          else if(z2048 < (LARGETILE_SIZE * 3))  FragColor = mix(texelFetch(textures[7], ivec2(0, yCoord), 0), texelFetch(textures[10], ivec2(LARGETILE_SIZE - 1, yCoord), 0), 0.5);
          else if(z2048 < 2048)  FragColor = mix(texelFetch(textures[8], ivec2(0, yCoord), 0), texelFetch(textures[11], ivec2(LARGETILE_SIZE - 1, yCoord), 0), 0.5);
          else                      FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }
      else FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}