#pragma once

namespace config
{
	struct PictureSize
	{
		float width;
		float height;
	};

	const PictureSize gamePixelSize = {
		16,
		16
	};

	const PictureSize textureSize = {
		256,
		256
	};
	
	const PictureSize gameFieldSize =
	{
		gamePixelSize.width * 10,
		gamePixelSize.height * 20
	};
}