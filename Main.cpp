# include <Siv3D.hpp> // Siv3D v0.6.16

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	const Font font1{ FontMethod::MSDF, 48, Typeface::Bold };
	const Font font2{ FontMethod::MSDF, 48 };
	const Font font3{ FontMethod::MSDF, 25 };
	constexpr double MAX_TIME = 60.0;
	double timeEnteredGamemode2 = 0.0;
	int gamemode = 0;
	int count_until_start = 3;
	double accumlatedTime = 0.0;
	int Answer_index = 0;
	String inputA, inputB, filtered;
	int userA, userB;
	int score = 0;
	// x^2 + ax + b;
	int a = 0, b = 0;
	constexpr double interval = 1.0;

	while (System::Update()) {
		switch (gamemode) {
		case -1:
			font1(U"やり方").draw(100, 100);
			font3(U"1. x² ± ax ± b の形の式が出てくるので、\n   (x ± A)(x ± B) の形に因数分解してください").draw(100, 150);
			font3(U"2. AまたはBが負の値の場合はマイナスを含めて\n  入力してください").draw(100, 225);
			font3(U"3. AまたはBの値を入力したらエンターキーを\n   押してください。次の項に移ります。\n　　※間違えてエンターを押すと戻れません。").draw(100, 300);
			font3(U"4. 間違えるとスコアが減ります。").draw(100, 400);
			font3(U"5. 制限時間は{}秒です。"_fmt(MAX_TIME)).draw(100, 450);
			if (SimpleGUI::Button(U"戻る", Vec2{ 450, 525 }, 200)) {
				gamemode = 0;
			}
			break;
		case 0:
			font1(U"とにかく因数分解").draw(100, 100);
			if (SimpleGUI::Button(U"スタート", Vec2{ 300, 400 }, 200)) {
				gamemode = 1;
				accumlatedTime = 0.0;
				count_until_start = 3;
			}
			if (SimpleGUI::Button(U"やり方", Vec2{ 300, 450 }, 200)) {
				gamemode = -1;
			}
			break;
		case 1:
			if (count_until_start > 0) {
				font2(U"{}"_fmt(count_until_start)).drawAt(400, 250);
				accumlatedTime += Scene::DeltaTime();
				if (interval <= accumlatedTime) {
					--count_until_start;
					accumlatedTime -= interval;
				}
			}
			else {
				accumlatedTime = 0;
				gamemode = 2;
				Answer_index = 3;
				score = 0;
				timeEnteredGamemode2 = Scene::Time();
			}
			break;
		case 2:
			if (Answer_index == 3) {
				a = Random(-19, 19);
				b = Random(-19, 19);
				Answer_index = 0;
				userA = 0;
				userB = 0;
				inputA.clear();
				inputB.clear();
			}
			else {
				String sign1 = (a + b >= 0) ? U"+" : U"-";
				String sign2 = (a * b >= 0) ? U"+" : U"-";
				font2(U"x² {} {}x {} {}"_fmt(sign1, Abs(a + b), sign2, Abs(a * b))).drawAt(400, 250);
				font2(U"(x + {})(x + {})"_fmt(inputA, inputB)).drawAt(400, 450);

				switch (Answer_index) {
				case 0:
					TextInput::UpdateText(inputA);
					// 新しい文字列に必要な文字だけをコピー
					filtered.clear();
					for (size_t i = 0; i < inputA.size(); ++i)
					{
						char32 ch = inputA[i];
						if (i == 0)
						{
							// 先頭は数字またはマイナスを許可
							if (IsDigit(ch) || ch == U'-')
							{
								filtered.push_back(ch);
							}
						}
						else
						{
							// 2文字目以降は数字のみ許可
							if (IsDigit(ch))
							{
								filtered.push_back(ch);
							}
						}
					}
					inputA = filtered;

					// 長さ制限：マイナス付きなら3文字、そうでなければ2文字
					if (inputA.starts_with(U'-') && inputA.size() > 3)
					{
						inputA = inputA.substr(0, 3);
					}
					else if (!inputA.starts_with(U'-') && inputA.size() > 2)
					{
						inputA = inputA.substr(0, 2);
					}
					// Enterで2つ目の入力へ移行
					if (KeyEnter.down() && !inputA.isEmpty()) {
						Answer_index = 1;
					}
					break;
				case 1:
					TextInput::UpdateText(inputB);
					filtered.clear();
					// 新しい文字列に必要な文字だけをコピー
					for (size_t i = 0; i < inputB.size(); ++i)
					{
						char32 ch = inputB[i];
						if (i == 0)
						{
							// 先頭は数字またはマイナスを許可
							if (IsDigit(ch) || ch == U'-')
							{
								filtered.push_back(ch);
							}
						}
						else
						{
							// 2文字目以降は数字のみ許可
							if (IsDigit(ch))
							{
								filtered.push_back(ch);
							}
						}
					}
					inputB = filtered;

					// 長さ制限：マイナス付きなら3文字、そうでなければ2文字
					if (inputB.starts_with(U'-') && inputB.size() > 3)
					{
						inputB = inputB.substr(0, 3);
					}
					else if (!inputB.starts_with(U'-') && inputB.size() > 2)
					{
						inputB = inputB.substr(0, 2);
					}

					if (KeyEnter.down() && !inputB.isEmpty())
					{
						userA = Parse<int>(inputA);
						userB = Parse<int>(inputB);

						if (((userA == a && userB == b) || (userA == b && userB == a))) {
							score += 1;
							Answer_index = 3;
						}
						else {
							score -= 1;
							Answer_index = 3;
						}
					}
				}
				font1(U"スコア: {}"_fmt(score)).draw(100, 100);
				font1(U"残り時間: {:.1f}"_fmt(fabs(MAX_TIME + timeEnteredGamemode2 - Scene::Time()))).draw(100, 150);
				if (MAX_TIME + timeEnteredGamemode2 - Scene::Time() <= 0.0) {
					gamemode = 3;
				}
			}
			break;
		case 3:
			font1(U"ゲーム終了").draw(100, 100);
			accumlatedTime += Scene::DeltaTime();
			if (2.0 <= accumlatedTime) {
				accumlatedTime = 0;
				gamemode = 4;
			}
			break;
		case 4:
			font1(U"あなたのスコア: {}"_fmt(score)).draw(100, 100);
			if (SimpleGUI::Button(U"終了", Vec2{ 300, 400 }, 200)) {
				gamemode = 0;
			}
			break;
		}
	}
}
