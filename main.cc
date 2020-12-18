/*1. Используя функции SDL (SDL_RenderDraw... и SDL_RenderFill...) нарисовать:
 *         - поверхность земли
 *         - траву
 *         - дом с крышей, окном, дверью и трубой
 *         - на заднем плане - забор (опционально)
 * 2. Реализовать рисование окружности с использованием алгоритма Брезенхэма
 * 3. Нарисовать у домика круглое окно на чердаке.
 */
#include <iostream>
#include <memory>
/*
 * std::shared_ptr - разделяемый указатель
 * std::unique_ptr - уникальный указатель
 * std::weak_ptr - "слабый" указатель
 */
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
constexpr int WIDTH = 1000, HEIGHT = 720, R = 25;

std::shared_ptr<SDL_Texture> icon;
std::shared_ptr<SDL_Texture> icon2;

std::shared_ptr<SDL_Texture> Make_icon(SDL_Renderer *renderer, const char *name)
{

	std::shared_ptr<SDL_Texture> picture = std::shared_ptr<SDL_Texture>(
			IMG_LoadTexture(renderer, name),
			SDL_DestroyTexture);
	if(picture == nullptr)
	{
		std::cerr << "Не могу загрузить картинку" <<
		SDL_GetError()<<std::endl;
		return nullptr;
	}
	return picture;
}


void DrawCircle(SDL_Renderer * ren, int r, int X, int Y)
{
			int x = 0;
			int y = r;
            int delta = 1 - 2 * r;
            int error = 0;


            while(y >= 0)
            {
                SDL_RenderDrawPoint(ren, X + x, Y + y);
                SDL_RenderDrawPoint(ren, X + x, Y - y);
                SDL_RenderDrawPoint(ren, X - x, Y + y);
                SDL_RenderDrawPoint(ren, X - x, Y - y);
                error = 2 * (delta + y) - 1;
                if (delta < 0 and error <= 0)
                    delta += 2 * ++x + 1;
                else  if (delta > 0 and error > 0)
                    delta -= 2 * --y + 1;
                else  delta += 2 * (++x - --y);
            }
}
int main(int, char **)
{
    std::cout << "Начало работы программы" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    auto win = std::shared_ptr<SDL_Window>(
            SDL_CreateWindow("Window",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    WIDTH, HEIGHT,
                    SDL_WINDOW_SHOWN),
            SDL_DestroyWindow);
    if (win == nullptr) {
        std::cerr << "Ошибка при создании окна: " <<
                SDL_GetError() << std::endl;
        return 1;
    }
    auto ren = std::shared_ptr<SDL_Renderer>(
            SDL_CreateRenderer(win.get(), -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
            SDL_DestroyRenderer);
    if (ren == nullptr) {
        std::cerr << "Ошибка при создании рендерера: " <<
                SDL_GetError() << std::endl;
        return 1;
    }


	bool f = true;
	int icon_x = 640, icon_y = 90;// координаты тома
	bool f2 = true;
	int icon_x2 = 500, icon_y2 = 500;// координаты джерри

	auto keys = SDL_GetKeyboardState(nullptr);

    SDL_Event event;
    bool finish = false;
    // Основной цикл программы
    while (not finish)
    {
        // Цикл обработки событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) // Если окно закрыли...
                finish = true;
        }
        //Небо
        SDL_SetRenderDrawColor(ren.get(), 108, 246, 250, 255);
        SDL_RenderClear(ren.get());
        //Земля
        SDL_Rect r { 0, 525, WIDTH, 220 };
        SDL_SetRenderDrawColor(ren.get(), 33, 168, 21, 255);
        SDL_RenderFillRect(ren.get(), &r);
        //забор
        for (int i = 20; i < WIDTH; i += 100) {
        	SDL_SetRenderDrawColor(ren.get(), 154, 89, 7, 255);
            SDL_Rect r1 { i, 450, 50, 100 };
            SDL_RenderFillRect(ren.get(), &r1);
            SDL_Rect r2 { i + 50, 490, 50, 20 };
            SDL_RenderFillRect(ren.get(), &r2);
          }
        //дом
        SDL_Rect r1 { WIDTH / 2 - 150, 200, 430, 430 };
        SDL_SetRenderDrawColor(ren.get(), 240, 217, 37, 255);
        SDL_RenderFillRect(ren.get(), &r1);
        //Окно
        SDL_Rect r2 { WIDTH / 2 + 25, 200 + 150, 100, 100 };
        SDL_SetRenderDrawColor(ren.get(), 194, 254, 244, 255);
        SDL_RenderFillRect(ren.get(), &r2);

        SDL_SetRenderDrawColor(ren.get(), 107, 75, 26, 1);
        SDL_RenderDrawLine(ren.get(), WIDTH / 2 + 75, 350, WIDTH / 2 + 75, 450);

        SDL_SetRenderDrawColor(ren.get(), 107, 75, 26, 1);
        SDL_RenderDrawLine(ren.get(), WIDTH / 2 + 25, 400, WIDTH / 2 + 125, 400);

        //Труба
        SDL_Rect r3 { WIDTH / 2 + 175, 130, 30, 60 };
        SDL_SetRenderDrawColor(ren.get(), 66, 45, 13, 1);
        SDL_RenderFillRect(ren.get(), &r3);
        //Крыша
        SDL_SetRenderDrawColor(ren.get(), 158, 85, 43, 255);
        for (double x1 = WIDTH / 2 + 65, x2 =WIDTH / 2 + 65,  y = 100; y < 200; x1 -= 2.15, x2 += 2.15, y++) {
            SDL_RenderDrawLine(ren.get(), x1, y, x2, y);
        }
        //дверь
        SDL_Rect r4 { WIDTH / 2 + 38 , 500, 75, 130 };
        SDL_SetRenderDrawColor(ren.get(), 66, 45, 13, 1);
        SDL_RenderFillRect(ren.get(), &r4);
        //Трава
        SDL_SetRenderDrawColor(ren.get(), 56, 255, 38, 1);
        for(int i = 0; i <= 25; i += 25)
            for (int x = 15 + i,  y1 = HEIGHT - i, y2 = HEIGHT - (i+50) ; x < WIDTH - 15; x += 50 )
                SDL_RenderDrawLine(ren.get(), x, y1, x, y2);

        //круглое окно на чердаке
        SDL_SetRenderDrawColor(ren.get(), 214, 250, 239, 98);
        for (int j = R; j > 0; j--){
        	DrawCircle(ren.get(),j, WIDTH / 2 + 65, 150);
        }


        //том
		if (f)
			icon = Make_icon(ren.get(), "tomRight.png");
		else
			icon = Make_icon(ren.get(), "tom1.png");

		if(keys[SDL_SCANCODE_RIGHT] and icon_x < WIDTH - 100){ icon_x += 10; f = true;}//тут и далее цифра 8 это скорость полета
		if(keys[SDL_SCANCODE_LEFT] and icon_x > 0 ){ icon_x -= 10; f = false;}
		if(keys[SDL_SCANCODE_UP] and icon_y > 0){ icon_y -= 10; }
		if(keys[SDL_SCANCODE_DOWN] and icon_y < HEIGHT - 50){ icon_y += 10; }

		if(icon == nullptr) return 1;
		SDL_Rect icon_rect {icon_x, icon_y, 1280/4, 720/4};
		SDL_RenderCopy(ren.get(), icon.get(), nullptr, &icon_rect);

		//джерри
		if(icon_x2 < WIDTH - 200 and f2){
			icon2 = Make_icon(ren.get(), "jerry1.png");
			icon_x2 += 15;//джерри бежит вправао
		}
		else f2 = false;
		if(icon_x2 > 0 and !f2){
			icon2 = Make_icon(ren.get(), "jerryLeft.png");
			icon_x2 -= 15;//джерри бежит влеао
		}
		else f2 = true;

		if(icon2 == nullptr) return 1;
		SDL_Rect icon_rect2 {icon_x2, icon_y2, 300, 300};
		SDL_RenderCopy(ren.get(), icon2.get(), nullptr, &icon_rect2);

        SDL_RenderPresent(ren.get());
        }
        /*
         * Двойная буферизация:
         * 1 буфер (передний) - буфер, содержимое которого отображается.
         * 2 буфер (задний) - буфер, в котором осуществляется построение изображения.
         *
         */
        std::cout << "Конец работы программы" << std::endl;
        return 0;
}
