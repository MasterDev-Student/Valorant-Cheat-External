using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using Immortal.SDK;
using Immortal.SDK.Extensions;
using Siticone.UI.WinForms;
using Siticone.UI.WinForms.Suite;

namespace Immortals
{
	// Token: 0x02000002 RID: 2
	public partial class MainForm : Form
	{
		// Token: 0x06000002 RID: 2 RVA: 0x00002058 File Offset: 0x00000258
		public MainForm()
		{
			this.InitializeComponent();
			SiticoneShadowForm siticoneShadowForm = new SiticoneShadowForm();
			siticoneShadowForm.SetShadowForm(this);
		}

		// Token: 0x06000003 RID: 3 RVA: 0x00002094 File Offset: 0x00000294
		private void MainForm_Load(object sender, EventArgs e)
		{
			try
			{
				MainThreads.thBunnyHopHandle.Start();
				MainThreads.thTriggerBotHandle.Start();
				MainThreads.thAutoPistolHandle.Start();
				this.overlayForm.Show();
				this.cbColor.SelectedIndex = 0;
				this.Text = new RandomGenerator().RandomString(8, true);
			}
			catch
			{
			}
		}

		// Token: 0x06000004 RID: 4 RVA: 0x00002108 File Offset: 0x00000308
		private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
		{
			Environment.Exit(-1);
		}

		// Token: 0x06000005 RID: 5 RVA: 0x00002112 File Offset: 0x00000312
		private void cbBunnyHop_CheckedChanged(object sender, EventArgs e)
		{
			MainThreads.BunnyHop = this.cbBunnyHop.Checked;
		}

		// Token: 0x06000006 RID: 6 RVA: 0x00002125 File Offset: 0x00000325
		private void cbTriggerBot_CheckedChanged(object sender, EventArgs e)
		{
			MainThreads.TriggerBot = this.cbTriggerBot.Checked;
		}

		// Token: 0x06000007 RID: 7 RVA: 0x00002138 File Offset: 0x00000338
		private void nudBunnyHopDelay_ValueChanged(object sender, EventArgs e)
		{
			MainThreads.BunnyHopDelay = (int)this.nudBunnyHopDelay.Value;
		}

		// Token: 0x06000008 RID: 8 RVA: 0x00002150 File Offset: 0x00000350
		private void nudTriggerbotRadius_ValueChanged(object sender, EventArgs e)
		{
			MainThreads.TriggerbotCrossRadius = (int)this.nudTriggerbotRadius.Value;
		}

		// Token: 0x06000009 RID: 9 RVA: 0x00002168 File Offset: 0x00000368
		private void nudTriggerbotShootDelay_ValueChanged(object sender, EventArgs e)
		{
			MainThreads.TriggerBotShotDelay = (int)this.nudTriggerbotShootDelay.Value;
		}

		// Token: 0x0600000A RID: 10 RVA: 0x00002180 File Offset: 0x00000380
		private void cbAutoStop_CheckedChanged(object sender, EventArgs e)
		{
			MainThreads.AutoStop = this.cbAutoStop.Checked;
		}

		// Token: 0x0600000B RID: 11 RVA: 0x00002193 File Offset: 0x00000393
		private void cbAutoPistol_CheckedChanged(object sender, EventArgs e)
		{
			MainThreads.AutoPistol = this.cbAutoPistol.Checked;
		}

		// Token: 0x0600000C RID: 12 RVA: 0x000021A6 File Offset: 0x000003A6
		private void nudAutoPistolDelay_ValueChanged(object sender, EventArgs e)
		{
			MainThreads.AutoPistolShotDelay = (int)this.nudAutoPistolDelay.Value;
		}

		// Token: 0x0600000D RID: 13 RVA: 0x000021C0 File Offset: 0x000003C0
		private void btnAutoPistolSetKey_Click(object sender, EventArgs e)
		{
			this.cmAutoPistolKey.Show(Cursor.Position.X, Cursor.Position.Y);
		}

		// Token: 0x0600000E RID: 14 RVA: 0x000021F4 File Offset: 0x000003F4
		private void zToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MainThreads.AutoPistolKey = Keys.Z;
			this.btnAutoPistolSetKey.Text = "Set Key - Z";
		}

		// Token: 0x0600000F RID: 15 RVA: 0x0000220F File Offset: 0x0000040F
		private void xButton1ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MainThreads.AutoPistolKey = Keys.XButton1;
			this.btnAutoPistolSetKey.Text = "Set Key - XButton1";
		}

		// Token: 0x06000010 RID: 16 RVA: 0x00002229 File Offset: 0x00000429
		private void xButton2ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MainThreads.AutoPistolKey = Keys.XButton2;
			this.btnAutoPistolSetKey.Text = "Set Key - XButton2";
		}

		// Token: 0x06000011 RID: 17 RVA: 0x00002243 File Offset: 0x00000443
		private void middleWheelToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MainThreads.AutoPistolKey = Keys.MButton;
			this.btnAutoPistolSetKey.Text = "Set Key - MButton";
		}

		// Token: 0x06000012 RID: 18 RVA: 0x0000225D File Offset: 0x0000045D
		private void cbDrawRadius_CheckedChanged(object sender, EventArgs e)
		{
			OverlayForm.DrawRadius = this.cbDrawRadius.Checked;
		}

		// Token: 0x06000013 RID: 19 RVA: 0x00002270 File Offset: 0x00000470
		private void cbColor_SelectedIndexChanged(object sender, EventArgs e)
		{
			bool flag = this.cbColor.SelectedIndex == 0;
			if (flag)
			{
				MainThreads.EnemyOutlineColor = 11801877;
			}
			else
			{
				bool flag2 = this.cbColor.SelectedIndex == 1;
				if (flag2)
				{
					MainThreads.EnemyOutlineColor = 10626210;
				}
				else
				{
					bool flag3 = this.cbColor.SelectedIndex == 2;
					if (flag3)
					{
						MainThreads.EnemyOutlineColor = 13158178;
					}
				}
			}
		}

		// Token: 0x04000001 RID: 1
		private OverlayForm overlayForm = new OverlayForm();
	}
}
