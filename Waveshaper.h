#include "UITools.h"
#include "resource.h"

typedef struct wavadatum
{   BOOL sinSwitch;
    BOOL cosSwitch;
	BOOL tanSwitch;
	BOOL squareSinSwitch;
	BOOL squareCosSwitch;
	BOOL limTanSwitch;
	BOOL notsquareSwitch;
	BOOL triSwitch;
	
	float amplitude;
	float cycles;
	float phase;

} wavedata;

typedef struct bankdatum
{	int blendAlgorithm;
	float favoriteResolution;	// used for presets.
} bankdata;



LRESULT CALLBACK Waveshape(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void paintOneOscillator(HDC hdc, int index);
void paintAllOscillators(HDC hdc);
void paintBlendedOscillators(HDC hdc, int blendAlgorithm);	// blend oscillators using specified algorithm.
void paintXvsY(HDC hdc);
void resetSettings(void);
void resetPresets(void);
void resetUI(HWND hDlg);
void restoreUI(HWND hDlg);
void initWavePresets(void);
void useWavePreset(HWND hDlg, int index);